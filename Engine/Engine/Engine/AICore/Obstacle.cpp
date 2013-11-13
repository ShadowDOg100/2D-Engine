#include "Obstacle.h"
#include "../GraphicsCore/GraphicsCore.h"
#include "Vector2D.h"

namespace AICore {
int ObstacleManager::CreateObstacle (float x, float y, float width, float height, bool solid, int spriteID, int gameSpecific)
{
	return obstacle.Add(new Obstacle(x, y, width, height, solid, spriteID, gameSpecific));
}

ObstacleManager::ObstacleManager(void):Observer()
{
}

ObstacleManager::~ObstacleManager(void)
{
}

void ObstacleManager::Draw()
{
	for(int i=0; i<obstacle.Top(); i++)
		if(obstacle[i])
			obstacle[i]->Draw();
}

void ObstacleManager::CheckCollision(float &tx, float &ty, float tw, float th, float &vx, float &vy, bool *sides)
{
	for(int i = 0; i < 4; i++) {
		sides[i] = false;
	}
	for(int i=0; i<obstacle.Top(); i++) {
		if( obstacle[i] && obstacle[i]->IsSolid() ) {
			obstacle[i]->CheckCollision(tx,ty,tw,th,vx,vy,sides);
		}
	}
}

void ObstacleManager::Flush() {
	obstacle.Flush();
}

bool LinesCross(float x1,float y1,float x2,float y2,
							float x3,float y3,float x4,float y4,float &crossX,float &crossY)
{
	//Vector2D s1(x1,y1),e1(x2,y2);
	//Vector2D s2(x3,y3),e2(x3,y4);
	if( max(x1,x2)<min(x3,x4) || max(y1,y2)<min(y3,y4) 
		|| min(x1,x2)>max(x3,x4) || min(y1,y2)>max(y3,y4) ) return false;
	// check verticle
	if( x3==x4 )
	{
		if( x1!=x2 && min(x1,x2)<x3 && max(x1,x2)>x3 )
		{
			crossX = x3;
			if( crossX != x1)
				crossY = y1 + (y2-y1)*((x3-x1)/(x2-x1));
			if( crossY < min(y3,y4) || crossY > max(y3,y4) ) return false;
			return true;
		}
		// check for inside - move to closest edge
	}
	else
	{ // check horizontal - this function requires second line to be horizontal or vertical
		if( y1!=y2 && min(y1,y2)<=y3 && max(y1,y2)>=y3 )
		{
			crossY = y3;
			if( crossY != y1)
				crossX = x1 + (x2-x1)*((y3-y1)/(y2-y1));
			if( crossX < min(x3,x4) || crossX > max(x3,x4) ) return false;
			return true;
		}
		// check for inside - move to closest edge
	}
	return false;
}

void Obstacle::CheckCollision(float &tx, float &ty, float tw, float th, float &dx, float &dy, bool *sides)
{
	float sx = tx - dx;
	float sy = ty - dy;
	if( dy > 0.0001 ) // moving Y+
	{
		if( sy+th == y && sx+tw >x && sx < x+width )
		{
			dy = 0;
			ty = sy;
			sides[2]=true;
		}
		else
		{
			float cx=tx,cy=ty;
			if( LinesCross(sx,sy+th,sx+dx,sy+th+dy,
							x,y,x+width,y,cx,cy) )
			{ // lines intersected
				tx = cx;
				ty = cy-th;
				dx = tx-sx;
				dy = ty-sy;
				sides[2] = true;
			}
			if( LinesCross(sx+tw,sy+th,sx+dx+tw,sy+th+dy,
							x,y,x+width,y,cx,cy) )
			{ // lines intersected
				tx = cx - tw;
				ty = cy - th;
				dx = tx - sx;
				dy = ty - sy;
				sides[2] = true;
			}
		}
	}
	else if( dy < -0.0001 ) // moving Y-
	{
		if( sy == y+height && sx+tw >x && sx < x+width )
		{
			dy = 0;
			ty = sy;
			sides[3]=true;
		}
		else
		{
			float cx=0,cy=0;
			if( LinesCross(sx,sy,sx+dx,sy+dy,
							x,y+height,x+width,y+height,cx,cy) )
			{ // lines intersected
				tx = cx;
				ty = cy;
				dx = tx - sx;
				dy = ty - sy;
				sides[3] = true;
			}
			if( LinesCross(sx+tw,sy,sx+dx+tw,sy+dy,
							x,y+height,x+width,y+height,cx,cy) )
			{ // lines intersected
				tx = cx - tw;
				ty = cy;
				dx = tx - sx;
				dy = ty - sy;
				sides[3] = true;
			}
		}
	}
	if( dx > 0.0001 ) // moving X+
	{
		if( sx+tw == x && sy+th >y && sy < y+height )
		{
			dx = 0;
			tx = sx;
			sides[0]=true;
		}
		else
		{
			float cx=0,cy=0;
			if( LinesCross(sx+tw,sy,sx+tw+dx,sy+dy,
							x,y,x,y+height,cx,cy) )
			{ // lines intersected
				tx = cx - tw;
				ty = cy;
				dx = tx - sx;
				dy = ty - sy;
				sides[0] = true;
			}
			if( LinesCross(sx+tw,sy+th,sx+dx+tw,sy+th+dy,
							x,y,x,y+height,cx,cy) )
			{ // lines intersected
				tx = cx - tw;
				ty = cy - th;
				dx = tx - sx;
				dy = ty - sy;
				sides[0] = true;
			}
		}
	}
	else if( dx < -0.0001 ) // moving X-
	{
		if( sx == x+width && sy+th >y && sy < y+height )
		{
			dx = 0;
			tx = sx;
			sides[1]=true;
		}
		else
		{
			float cx=0,cy=0;
			if( LinesCross(sx,sy,sx+dx,sy+dy,
							x+width,y,x+width,y+height,cx,cy) )
			{ // lines intersected
				tx = cx;
				ty = cy;
				dx = tx - sx;
				dy = ty - sy;
				sides[1] = true;
			}
			if( LinesCross(sx,sy+th,sx+dx,sy+th+dy,
							x+width,y,x+width,y+height,cx,cy) )
			{ // lines intersected
				tx = cx;
				ty = cy - th;
				dx = tx - sx;
				dy = ty - sy;
				sides[1] = true;
			}
		}
	}
	////if((ty+th)-(th/5.0) < y || ty + (th/5.0) > y+height) {
	////	if(vy > 0) { //down
	////		if(ty+th > y && ty+th < y+height && !(tx+tw <= x || tx >= x+width)) {
	////			ty -= (ty+th) - y;
	////			sides[2] = true;
	////		}
	////	}
	////	if(vy < 0) { //up
	////		if(ty < y+height && ty > y && !(tx+tw <= x || tx >= x+width)) {
	////			ty += (y+height) - ty;
	////			sides[3] = true;
	////		}
	////	}
	////} else {
	////	if(vx > 0) { //right
	////		if(tx+tw > x && tx+tw < x+width && !(ty+th <= y || ty >= y+height)) {
	////			tx -= (tx + tw) - x;
	////			sides[0] = true;
	////		}
	////	}
	////	if(vx < 0) { //left
	////		if(tx > x && tx < x+width && !(ty+th <= y || ty >= y+height)) {
	////			tx += (x+width) - tx;
	////			sides[1] = true;
	////		}
	////	}
	////}
}

void Obstacle::Draw() {
	if(spriteID >= 0) {
		GraphicsCore::DrawSprite(spriteID,(int)width,(int)height, (int)x, (int)y);
	}
}
};
