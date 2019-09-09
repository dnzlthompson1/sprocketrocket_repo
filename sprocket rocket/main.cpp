#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;
using std::vector;

unsigned int alien_tex, rocket_tex, char_tex, sprocket_tex, background_tex, load_tex;


class Screen
{
public:
    float top=600,bottom=0,left=0,right=1000;
};
class Block
{
 public:
    int x,y;
    Block (){}
    void draw()
    {
        float height=10, width=40;
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(0,0,1);
        glBegin(GL_QUADS);
            glVertex2d(x,y);
            glVertex2d(x+width,y);
            glVertex2d(x+width,y+height);
            glVertex2d(x,y+height);
        glEnd();
        glColor3f(1,1,1);
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    };
};
class Rocket
{
 public:
    int frame=1;
    float x=500,y=50;
    int health=100;
    void draw()
    {
        frame++;
        if(frame>9)frame=1;

        glPushMatrix();
        glRotatef(0,0,0,0);
        glTranslatef(0,0,0);
        glScalef(1,1,1);
        glBindTexture(GL_TEXTURE_2D,rocket_tex);
        glBegin(GL_QUADS);
            glTexCoord2d(.1111*(frame-1),0);
            glVertex2d(x-30,y+70);
            glTexCoord2d(.1111*(frame-1),1);
            glVertex2d(x-30,y-70);
            glTexCoord2d(.1111*frame,1);
            glVertex2d(x+30,y-70);
            glTexCoord2d(.1111*frame,0);
            glVertex2d(x+30,y+70);
        glEnd();
        glPopMatrix();
    }
    void moveRight()
    {
        x+=10;
    }
    void moveLeft()
    {
        x-=10;
    }
    void fireSprocket()
    {

    }
};
class Sprocket
{
 public:
    float x,y;
    float dirx,diry;
    Sprocket(float mouse_x,float mouse_y,Rocket rocket)
    {
        x=rocket.x;
        y=rocket.y;
        dirx=(mouse_x-rocket.x)/sqrt(pow(mouse_x-rocket.x,2)+pow(mouse_y-rocket.y,2));
        diry=(mouse_y-rocket.y)/sqrt(pow(mouse_x-rocket.x,2)+pow(mouse_y-rocket.y,2));
    }
    void draw()
    {
        glBindTexture(GL_TEXTURE_2D,sprocket_tex);
        glBegin(GL_QUADS);
            glTexCoord2d(0,0);
            glVertex2d(x-20,y+20);
            glTexCoord2d(0,1);
            glVertex2d(x-20,y-20);
            glTexCoord2d(1,1);
            glVertex2d(x+20,y-20);
            glTexCoord2d(1,0);
            glVertex2d(x+20,y+20);
        glEnd();
    }
    void update()
    {
        x+=10*dirx;
        y+=10*diry+2;
    }
};
class Alien
{
 public:
    int jump_point_x,jump_point_y;
    Block ablo;
    int x,y;
    bool jump=false;
    int flatspeed;
    bool alive=true;
    int upspeed=25;
    Alien()
    {
        ablo.x=-20;
        ablo.y=-20;
    }
    void draw()
    {
        if(alive)
        {
            glBindTexture(GL_TEXTURE_2D, alien_tex);
            glBegin(GL_QUADS);
                glColor3ub(255,255,255);
                glTexCoord2f(0.0,0.0);
                glVertex2f(x-25,y+25);
                glTexCoord2f(0.0,1.0);
                glVertex2f(x-25,y-25);
                glTexCoord2f(1.0,1.0);
                glVertex2f(x+25.0,y-25);
                glTexCoord2f(1.0,0.0);
                glVertex2f(x+25,y+25);
            glEnd();
        }
        ablo.draw();
	}

	void resurect()
	{
	    alive=true;
	    upspeed=25;
	    jump=false;
	    jump_point_y+=800;
	    x=jump_point_x;
	    y=jump_point_y;
	}
	void update(Rocket rocket,Screen scr,int* rochel)
	{
	    ablo.x=jump_point_x-22;ablo.y=jump_point_y-28;
	    if(y<rocket.y+150&&rocket.x<x+100&&rocket.x>x-100) jump=true;
	    if(y>rocket.y-20&&y<rocket.y+20&&rocket.x<x+10&&rocket.x>x-10)
	    {
	        if(alive)*rochel-=5;
            alive=false;
	    }
        if (jump)
        {
            upspeed-=1.5; y+=upspeed; x+=flatspeed*5;
        }
	    if(rocket.x>x)flatspeed=1;
        else if(rocket.x<x)flatspeed=-1;
        else flatspeed=0;
	    if(ablo.y+100<scr.bottom)resurect();
	}
};
class Alien1
{
 public:
    int x,y;
    bool leftsided; //determines what side of the screen it appears
    bool alive=true;
    int upspeed=0;
    Alien1(Screen scr,bool ls)
    {
        leftsided=ls;
        y=scr.top+50;
        if(leftsided==true)x=30;
            else x=970;
    }
    void draw()
    {
        if(alive)
        {
            glBindTexture(GL_TEXTURE_2D, alien_tex);
            glBegin(GL_QUADS);
                glColor3ub(255,255,255);
                glTexCoord2f(0.0,0.0);
                glVertex2f(x-25,y+25);
                glTexCoord2f(0.0,1.0);
                glVertex2f(x-25,y-25);
                glTexCoord2f(1.0,1.0);
                glVertex2f(x+25.0,y-25);
                glTexCoord2f(1.0,0.0);
                glVertex2f(x+25,y+25);
            glEnd();
        }
	}
	void update(Rocket rocket,Screen scr,int* rochel)
	{
	    if(y>rocket.y+150||y>scr.top-30)upspeed=0;
        else if(y<rocket.y-150||y<scr.bottom+20) upspeed=4;
	    if(y>rocket.y-20&&y<rocket.y+20&&rocket.x<x+10&&rocket.x>x-10)
	    {
	        if(alive)*rochel-=5;
            alive=false;
	    }
	    y+=upspeed;
	}
};

GLuint loadTextureAlpha( const std::string &fileName )
{
    SDL_Surface *image = IMG_Load( fileName.c_str() );
    SDL_DisplayFormatAlpha(image);
    unsigned object(0);
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    SDL_FreeSurface(image);
  return object;
}
GLuint loadTexture( const std::string &fileName )
{
    SDL_Surface *image = IMG_Load( fileName.c_str() );
    SDL_DisplayFormat(image);
    unsigned object(0);
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    SDL_FreeSurface(image);
  return object;
}
void write(char text[],int x=50,int line=1)
{
    int y=600-(15*line);
    for ( unsigned int charplace=1; text[charplace-1]!='\0'; charplace++)
    {
        float ytex=0.01029;
        int charnum=text[charplace-1]-31;
        glBindTexture(GL_TEXTURE_2D, char_tex);
        glBegin(GL_QUADS);
            glTexCoord2f(1,ytex*(charnum-1));
            glVertex2f(x+(10*charplace),y+15);
            glTexCoord2f(1,ytex*charnum);
            glVertex2f(x+(10*charplace),y);
            glTexCoord2f(0,ytex*charnum);
            glVertex2f(x+(10*(charplace-1)),y);
            glTexCoord2f(0,ytex*(charnum-1));
            glVertex2f(x+(10*(charplace-1)),y+15);
        glEnd();
    }
}
void write(float number,int x=50,int line=1)
{

    string text;          // string which will contain the result
    ostringstream convert;   // stream used for the conversion
    convert << number;      // insert the textual representation of 'Number' in the characters in the stream
    text = convert.str();
    int y=600-(15*line);
    for ( unsigned int charplace=1; text[charplace-1]!='\0'; charplace++)
    {
        float ytex=0.01029;
        int charnum=text[charplace-1]-31;
        glBindTexture(GL_TEXTURE_2D, char_tex);
        glBegin(GL_QUADS);
            glTexCoord2f(1,ytex*(charnum-1));
            glVertex2f(x+(10*charplace),y+15);
            glTexCoord2f(1,ytex*charnum);
            glVertex2f(x+(10*charplace),y);
            glTexCoord2f(0,ytex*charnum);
            glVertex2f(x+(10*(charplace-1)),y);
            glTexCoord2f(0,ytex*(charnum-1));
            glVertex2f(x+(10*(charplace-1)),y+15);
        glEnd();
    }
}

class Game
{
public:
    bool up_press=false ,down_press=false,space_press=false,
    right_press=false,left_press=false,left_mouse_press=false;
    int lmp= 0;
    int frame=0;
    bool game_start=false;
    float xcord=0, ycord=0, ycord2=0;
    int mouse_x=0, mouse_y=0;
    int mouse_x2=0, mouse_y2=0;
    Screen scr;

    vector<Alien*> alien;
    deque<Sprocket*> sprocket;
    deque<Alien1*> alien1;

    Rocket rocket;
    void pregame()
    {
        bool loading=true;
        while(loading)
            {
                Uint32 start;
                start=SDL_GetTicks();
                glClear(GL_COLOR_BUFFER_BIT);

                glBindTexture(GL_TEXTURE_2D, load_tex);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0,0.0);
                    glVertex2f(0,600);
                    glTexCoord2f(0.0,1.0);
                    glVertex2f(0,0);
                    glTexCoord2f(1.0,1.0);
                    glVertex2f(1000,0);
                    glTexCoord2f(1.0,0.0);
                    glVertex2f(1000,600);
                glEnd();

                SDL_GL_SwapBuffers();
                if(1000/30>(SDL_GetTicks()-start))
                    SDL_Delay(1000/30-(SDL_GetTicks()-start));
            }
    }
    void collisions()
    {
        for(int i=0;i<sprocket.size()&&sprocket.size()>0;i++)
            {
                for(int f=0;f<alien.size()&&alien.size()>0;f++)
                {
                    if(sprocket[i]->y>alien[f]->y-25&&sprocket[i]->y<alien[f]->y+25
                        &&sprocket[i]->x>alien[f]->x-25&&sprocket[i]->x<alien[f]->x+25)
                    {
                        alien[f]->alive=false;
                    }
                }
                for(int f=0;f<alien1.size()&&alien1.size()>0;f++)
                {
                    if(sprocket[i]->y>alien1[f]->y-25&&sprocket[i]->y<alien1[f]->y+25
                        &&sprocket[i]->x>alien1[f]->x-25&&sprocket[i]->x<alien1[f]->x+25)
                    {
                        alien1[f]->alive=false;
                    }
                }
            }
            for(int i=0;i<alien1.size()&&alien1.size()>1;i++)
            {
                for(int d=i;d<alien1.size();d++)
                {
                    if(alien1[i]->leftsided==alien1[d]->leftsided&&alien1[d]->y<alien1[i]->y+40&&i!=d&&alien1[d]->upspeed==0)
                    {
                        alien1[d]->upspeed=4;
                    }
                    if(alien1[i]->leftsided==alien1[d]->leftsided&&alien1[i]->upspeed==4&&alien1[i]->y>alien1[d]->y-40&&i!=d)
                    {
                            alien1[i]->upspeed=0;
                    }
                }

            }
            if(sprocket.size()>0)
            {
                if((sprocket[0]->y>scr.top+20||sprocket[0]->y<scr.bottom||sprocket[0]->x>scr.right||sprocket[0]->x<scr.left))
                {
                    sprocket.pop_front();
                }
            }
    }

    void init()
    {
        glClearColor(0.4,0.6,1.0,1.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,1000,0,600);
        SDL_Init(SDL_INIT_EVERYTHING);

        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
        SDL_WM_SetCaption( "Sprocket Rocket", NULL);
        SDL_WM_SetIcon(SDL_LoadBMP("cb.bmp"), NULL);

        glDisable(GL_DEPTH_TEST);
        glEnable( GL_BLEND );
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //load_tex = loadTexture("load.png");
        //pregame();
        alien_tex = loadTextureAlpha("alien.png");
        rocket_tex= loadTextureAlpha("newrocket.png");
        sprocket_tex= loadTextureAlpha("sprocket.png");
        char_tex= loadTextureAlpha("char2.png");
        background_tex=loadTextureAlpha("backdrop.png");

        for(int i=0;i<3;i++)
        {
            alien.push_back(new Alien());
        }
        int alienX[]={300,400,500};
        int alienY[]={1000,900,800};
        for(int i=0;i<alien.size();i++)
        {
            alien[i]->jump_point_x=alienX[i];
            alien[i]->jump_point_y=alienY[i];
            alien[i]->x=alienX[i];
            alien[i]->y=alienY[i];
        }
    }
    void updatescene()
    {
        up_press==SDLK_w;
        if (space_press)game_start=true;
        if (game_start)
        {
            ycord-=1 ;
            if(ycord<=-(60000/33))ycord=0;
            ycord2-=1 ;
            if(ycord2<=-(60000/33))ycord2=0;

            frame++;

            for(int i=0;i<alien.size();i++)
            {
                alien[i]->update(rocket,scr,&rocket.health);
            }
            for(int i=0;i<alien1.size();i++)
            {
                alien1[i]->update(rocket,scr,&rocket.health);
            }
            for(int i=0;i<sprocket.size();i++)
            {
                sprocket[i]->update();
            }

            for(int i=0;i<alien1.size()&&alien1.size()>0;i++)
            {
                if(alien1[0]->alive)alien1.push_back(alien1[0]);
                alien1.pop_front();
            }
            //collisions////////////////////////////////////////////////////////
            collisions();                                                     //
            ////////////////////////////////////////////////////////////////////
            mouse_y+=2;
            scr.bottom+=2;
            scr.top+=2;
            if(left_mouse_press)lmp++;
            if(lmp==1)sprocket.push_back(new Sprocket(mouse_x, mouse_y,rocket));
            if(frame==501)alien1.push_back(new Alien1(scr,true));
            if(alien1.size()>0)
            {
                if(frame%500==0&&alien1.size()<8&&alien1[alien1.size()-1]->leftsided==true)alien1.push_back(new Alien1(scr,false));
                else if(frame%500==0&&alien1.size()<8&&alien1[alien1.size()-1]->leftsided==false)alien1.push_back(new Alien1(scr,true));
            }
            if(right_press&&rocket.x<1000)rocket.moveRight();
            if(left_press&&rocket.x>0)rocket.moveLeft();
            int rockup=2;
            if (up_press)rockup=10;
            else rockup=2;
            if (down_press)rockup=-6;
            rocket.y+=rockup;
        }
    }
    void display()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glPushMatrix();
        glTranslatef(0,0,0);

        glBindTexture(GL_TEXTURE_2D, background_tex);
        glBegin(GL_QUADS);
            glColor3ub(255,255,255);

            glTexCoord2f(0.0,0.0);
            glVertex2f(xcord,ycord+(60000/33)+(60000/33));
            glTexCoord2f(0.0,1.0);
            glVertex2f(xcord,ycord+(60000/33));
            glTexCoord2f(1.0,1.0);
            glVertex2f(xcord+(33000/33),ycord+(60000/33));
            glTexCoord2f(1.0,0.0);
            glVertex2f(xcord+(33000/33),ycord+(60000/33)+(60000/33));

            glTexCoord2f(0.0,0.0);
            glVertex2f(xcord,ycord2+(60000/33));
            glTexCoord2f(0.0,1.0);
            glVertex2f(xcord,ycord2);
            glTexCoord2f(1.0,1.0);
            glVertex2f(xcord+(33000/33),ycord2);
            glTexCoord2f(1.0,0.0);
            glVertex2f(xcord+(33000/33),ycord2+(60000/33));

        glEnd();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,-scr.bottom,0);

        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, alien_tex);
        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glTexCoord2f(0.0,0.0);
            glVertex2f(mouse_x,mouse_y);
            glTexCoord2f(0.0,1.0);
            glVertex2f(mouse_x,mouse_y-50);
            glTexCoord2f(1.0,1.0);
            glVertex2f(mouse_x+50.0,mouse_y-50);
            glTexCoord2f(1.0,0.0);
            glVertex2f(mouse_x+50,mouse_y);
        glEnd();
        glPopMatrix();

        for(int i=0;i<alien.size();i++ )
        {
            alien[i]->draw();
        }
        for(int i=0;i<alien1.size();i++ )
        {
            alien1[i]->draw();
        }
        for(int i=0;i<sprocket.size();i++ )
        {
            sprocket[i]->draw();
        }
        rocket.draw();

        glPopMatrix();
        write("rockstars [R] US",10,2);
        write(alien[0]->ablo.x,10,3);write(alien[0]->ablo.y,140,3);
        write("alien1#2 left",10,4);if(alien1.size()>1)write(alien1[1]->leftsided,170,4);
        write("rocket health",10,5); write( rocket.health,150,5);
        write("# of alien1:",10,6); write(alien1.size(),170,6);
        write("alien1 y",10,7); if(alien1.size()>0)write(alien1[0]->y,170,7);
        if (!game_start) write("press space to start game",390,19 );
    }

    Game()
    {
        SDL_Surface* screen;
        //screen=SDL_SetVideoMode(1366,768,32,SDL_FULLSCREEN|SDL_OPENGL);
        screen=SDL_SetVideoMode(1000,600,32,SDL_SWSURFACE|SDL_OPENGL);
        bool running=true;
        Uint32 start;
        SDL_Event event;
        init();

        //removes mouse
        //SDL_ShowCursor(0);
        while(running)
        {
            start=SDL_GetTicks();
            while(SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT) {running=false;}
                if (event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_UP:
                            up_press=true;
                            break;
                        case SDLK_DOWN:
                            down_press=true;
                            break;
                        case SDLK_SPACE:
                            space_press=true;
                            break;
                        case SDLK_RIGHT:
                            right_press=true;
                            break;
                        case SDLK_LEFT:
                            left_press=true;
                            break;
                        case SDLK_w:
                            up_press=true;
                            break;
                        case SDLK_s:
                            down_press=true;
                            break;
                        case SDLK_d:
                            right_press=true;
                            break;
                        case SDLK_a:
                            left_press=true;
                            break;
                        default:
                            break;
                    }
                }
                if (event.type == SDL_KEYUP)
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            up_press=false;
                            break;
                        case SDLK_DOWN:
                            down_press=false;
                            break;
                        case SDLK_SPACE:
                            space_press=false;
                            break;
                        case SDLK_w:
                            up_press=false;
                            break;
                        case SDLK_s:
                            down_press=false;
                            break;
                        case SDLK_RIGHT:
                            right_press=false;
                            break;
                        case SDLK_LEFT:
                            left_press=false;
                            break;
                        case SDLK_d:
                            right_press=false;
                            break;
                        case SDLK_a:
                            left_press=false;
                            break;
                        default:
                            break;
                    }
                }
                if (event.type == SDL_MOUSEMOTION)
                {
                    mouse_x = event.motion.x;
                    mouse_y = -event.motion.y+600+scr.bottom;
                }
                if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    switch(event.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        left_mouse_press=true;
                        break;
                    default:
                        break;
                    }
                }
                if (event.type == SDL_MOUSEBUTTONUP)
                {
                    switch(event.button.button)
                    {
                    case SDL_BUTTON_LEFT:
                        left_mouse_press=false;
                        lmp=0;
                        break;
                    default:
                        break;
                    }
                }
            }

            updatescene();
            display();
            SDL_GL_SwapBuffers();
            if(1000/30>(SDL_GetTicks()-start))
                SDL_Delay(1000/30-(SDL_GetTicks()-start));
        }
        SDL_Quit();
    }
    ~Game()
    {
        for(int i=0;i<alien.size();i++)
        {
            delete alien[i];
            alien[i]=0;
        }
        for(int i=0;i<alien1.size();i++)
        {
            delete alien1[i];
            alien1[i]=0;
        }
        for(int i=0;i<sprocket.size();i++)
        {
            delete sprocket[i];
            sprocket[i]=0;
        }
    }
};
int main(int main,char** argv)
{
    Game game;
	return 0;
}
