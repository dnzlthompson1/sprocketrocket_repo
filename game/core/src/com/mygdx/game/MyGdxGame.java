package com.mygdx.game;
import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Camera;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import java.util.ArrayDeque;

public class MyGdxGame extends ApplicationAdapter{
	private SpriteBatch batch;
	private BitmapFont font;
	private Texture imgrocket;
	private Texture imgalien;
	private Texture imgsprocket;
	private Texture imgbackdrop;
	private Alien[] alien = new Alien[3];
	private Rocket rocket = new Rocket();
	private ShapeRenderer shapeRenderer;
	private float ax, ay, az;
	private int frame=0;
	private boolean game_start=false;
	private float xcord=0, ycord=0, ycord2=0;
	private int click_x=0, click_y=0;
	int mouse_x2=0, mouse_y2=0;
	private Screen scr= new Screen();
	ArrayDeque<Sprocket> sprockets = new ArrayDeque<Sprocket>();
	private boolean touch=false;
	private boolean letgo=true;
	private boolean[] ar_collid = {false,false,false};
	private boolean[] ar_dellid = {true,true,true};

	@Override
	public void create(){
		shapeRenderer = new ShapeRenderer();
		batch = new SpriteBatch();
		imgrocket = new Texture("rocket.png");
		imgalien = new Texture("alien.png");
		imgsprocket = new Texture("sprocket.png");
		imgbackdrop = new Texture("backdrop.png");
		font = new BitmapFont(Gdx.files.internal("arial.fnt"));
		Gdx.graphics.setWindowedMode(1080, 2200);

		int alienX[]={300,400,500};
		int alienY[]={1000,900,800};
		for(int i=0;i<alien.length;i++)	{
			alien[i]=new Alien(alienX[i],alienY[i]);
		}
	}

	void updatescene(){
		ax = Gdx.input.getAccelerometerX();
		ay = Gdx.input.getAccelerometerY();
		az = Gdx.input.getAccelerometerZ();
		if (Gdx.input.isTouched())game_start=true;
		click_x=Gdx.input.getX();
		click_y=-Gdx.input.getY()+2200;
		touch =Gdx.input.isTouched();

		if (game_start){
			Sprocket frspoc=sprockets.peek();
			if(frspoc != null){
				if(Math.abs(frspoc.x)>3000 || Math.abs(frspoc.y-scr.bottom)>3000){
					sprockets.pop();
				}
			}
			ycord-=1;
			if(ycord<=-(60000/33))ycord=0;
			ycord2-=1 ;
			if(ycord2<=-(60000/33))ycord2=0;

			frame++;
			scr.bottom+=3;
			scr.top+=3;

			rocket.update();
			for(int i=0;i<alien.length;i++){
				alien[i].update(rocket,scr);
			}
			for( Sprocket sproc:sprockets){
				sproc.update();
			}
			if(touch && letgo)
			{
				rocket.fireSprocket();
			}
			if(!touch)
			{
				letgo=true;
			}else{
				letgo=false;
			}

			//collisions//////////////////////////////////////////////////////////////////////////////
			//collisions();
			//////////////////////////////////////////////////////////////////////////////////////////


		}
	}

	void display(){
		rocket.draw();
		for(int i=0;i<alien.length;i++ ){
			alien[i].draw();
		}
		for( Sprocket sproc:sprockets){
			sproc.draw();
		}

		batch.begin();
		font.draw(batch,"AX: " + Float.toString(ax),300,300);
		font.draw(batch,"AY: " + Float.toString(ay),300,260);
		font.draw(batch,"AZ: " + Float.toString(az),300,220);
		font.draw(batch,"click x:"+ Float.toString(click_x)+ " y:" + Float.toString(click_y),300,180);
		font.draw(batch,"rocket health:"+ Float.toString(rocket.health),300,140);
		font.draw(batch,"ar_collid[0]:"+ Boolean.toString(ar_collid[0])+ " ar_delid[0]:"+ Boolean.toString(ar_dellid[0]),300,100);

		batch.end();
	}
	public void render(){
		Gdx.gl.glClearColor(1, (float) 0.5, 0, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

		updatescene();
		display();
	}
	@Override
	public void dispose(){
		batch.dispose();
		imgrocket.dispose();
		imgsprocket.dispose();
		imgbackdrop.dispose();
		imgalien.dispose();
		//font.dispose();
		shapeRenderer.dispose();
	}
	class Screen{
		int top=2000,bottom=0,left=0,right=1200;
	};
	class Block{
		float x,y;
		Block (float bx,float by){x=bx;y=by;}
		void draw(){
			shapeRenderer.begin(ShapeRenderer.ShapeType.Filled);
			shapeRenderer.rect(x-20,y-scr.bottom-6,40,12,Color.BLUE,Color.BLUE,Color.BLUE,Color.BLUE);
			shapeRenderer.end();
		};
	};
	class Rocket{
		private int frame;
		float x=500,y=50;
		int health=100;
		int upspeed=3;
		Rocket(){
			frame = 1;
		}

		void draw(){
			frame++;
			if(frame>9)frame=1;
			batch.begin();
			batch.draw(imgrocket, x-25, y-scr.bottom-65,50,130);
			batch.end();
		}
		void moveRight(){
			x+=10;
		}
		void update(){
			y+=upspeed;
			if(ax>4){
				moveLeft();
			}
			if(ax<-4){
				moveRight();
			}
		}
		void moveLeft(){
			x-=10;
		}
		void fireSprocket(){
			sprockets.add(new Sprocket(click_x,click_y,rocket,scr));
		}
	};
	class Sprocket{
		float x,y;
		public float getx(){return x;}
		public float gety(){return y;}
		float dirx,diry;
		Sprocket(float click_x,float click_y,Rocket rocket,Screen scr){
			x=rocket.x;
			y=rocket.y;
			dirx= (float) ((click_x-rocket.x)/Math.sqrt(Math.pow(click_x-rocket.x,2)+Math.pow(click_y-(rocket.y-scr.bottom),2)));
			diry= (float) ((click_y-(rocket.y-scr.bottom))/Math.sqrt(Math.pow(click_x-rocket.x,2)+Math.pow(click_y-(rocket.y-scr.bottom),2)));
		}
		void draw(){
			batch.begin();
			batch.draw(imgsprocket, x-45, y-45-scr.bottom,70,70);
			batch.end();
		}
		void update(){
			x+=10*dirx;
			y+=10*diry;
		}
	};
	class Alien{
		private int jump_point_x;
		private int jump_point_y;
		private Block ablo;
		private int x;
		private int y;
		private boolean jump=false;
		private int flatspeed;
		private boolean alive=true;
		private int upspeed=25;
		Alien(int ax, int ay){
			x=ax;y=ay;jump_point_x=ax;jump_point_y=ay;
			ablo= new Block(jump_point_x,jump_point_y-30);
		}
		void draw(){
			if(alive){
				batch.begin();
				batch.draw(imgalien, x-25, y-scr.bottom-25,50,50);
				batch.end();
			}
			ablo.draw();
		}

		private void resurect(){
			alive=true;
			upspeed=25;
			jump=false;
			jump_point_y+=800;
			x=jump_point_x;
			y=jump_point_y;
		}
		void update(Rocket rocket,Screen scr){
			try{
				ablo.x = jump_point_x;
				ablo.y = jump_point_y-30;
				if (y < rocket.y + 150 && rocket.x < x + 100 && rocket.x > x - 100) jump = true;
				if (y > rocket.y - 20 && y < rocket.y + 20 && rocket.x < x + 10 && rocket.x > x - 10){
					//if (alive) rocket.health -= 5;
					alive = false;
				}
				if (rocket.x - 50 < x && rocket.x + 50 > x && rocket.y - 50 < y && rocket.y + 50 > y){
					ar_collid[0]= true;
				}else{
					ar_collid[0]= false;
				}
				if(ar_collid[0] && ar_dellid[0]){
					rocket.health -= 1;
				}
				if(!ar_collid[0]){
					ar_dellid[0]=true;
				}else{
					ar_dellid[0]=false;
				}
				if (jump){
					if (upspeed>-30){upspeed -= 1.5;}
					y += upspeed;
					x += flatspeed * 5;
				}
				if (rocket.x > x) flatspeed = 1;
				else if (rocket.x < x) flatspeed = -1;
				else flatspeed = 0;
				if (ablo.y + 100 < scr.bottom) resurect();
				System.out.println(">"+x+y);
			}
			catch (Exception e){
				Gdx.app.log("",""+e);
				System.out.println("j"+e);
			}
		}
	};
	class Alien1{
		private int x,y;
		private boolean alive=true;
		private int upspeed=0;
		Alien1(Screen scr,boolean ls){
			y= (int) (scr.top+50);
			if(ls) x = 30;
			else x=970;
		}
		void draw(){
			if(alive){
				/*draw stuff-scr.bottom*/
			}
		}
		void update(Rocket rocket,Screen scr){
			if(y>rocket.y+150||y>scr.top-30)upspeed=0;
			else if(y<rocket.y-150||y<scr.bottom+20) upspeed=4;
			y+=upspeed;
		}
	};
}


