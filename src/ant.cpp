#include <ant.h>
#include <sinCosLookup.h>

Ant::Ant(float x, float y, float theta, float size, float velocity, float placedPheromoneIntensity)
{
	_x = x;
	_y = y;
	_theta = theta;
	_size = size;
	_velocity = velocity;
}

void Ant::move(int l)
{
	float angle = _theta/M_PI*180;
	while (angle < 0) angle += 360;
	while (angle > 360) angle -= 360;

	_x += _velocity*cosLookup[(int)angle*10];
	_y += _velocity*sinLookup[(int)angle*10];

	//_x += _velocity*cos(_theta);
	//_y += _velocity*sin(_theta);
	//TODO sin e cos lookup

	
	if(l%300 == 0)
	{
		if(rand()%2 == 0)_theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*1.0f;
		else _theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*1.0f;
	}

}

void Ant::putPheromone(int* pheromoneMatrix)
{
/*
    bitset<32> r = 255;
    bitset<32> g = 0;
    bitset<32> b = 255;
    bitset<32> a = 0;
    
    bitset<32> rgba = ((a<<24)|(b<<16)|(g<<8)|r);

    bitset<32> r2 = rgba & (bitset<32>)255;
    bitset<32> g2 = rgba>> 8 & (bitset<32>) 255;
    bitset<32> b2 = rgba >> 16 & (bitset<32>) 255;
    bitset<32> a2 = rgba >> 24 &(bitset<32> )255;

    float xn, yn;
    xn = _x < 0 ? (SCR_WIDTH/2) - (-_x*(SCR_WIDTH/2)) : _x*(SCR_WIDTH/2)+(SCR_WIDTH/2);
    yn = _y < 0 ? (SCR_HEIGHT/2) - (-_y*(SCR_HEIGHT/2)) : _y*(SCR_HEIGHT/2)+(SCR_HEIGHT/2);

    pheromoneMatrix[((SCR_HEIGHT-(int)yn)*SCR_WIDTH+(int)xn)] = (GLbitfield)rgba.to_ulong();*/
}

void Ant::draw()
{

}

void Ant::run()
{

}
	
