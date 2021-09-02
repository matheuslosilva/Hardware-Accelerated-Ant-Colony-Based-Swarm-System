#include <ant.h>

#include <environment.h>



Ant::Ant(float x, float y, float theta, float size, float velocity, float placedPheromoneIntensity)
{
	_x = x;
	_y = y;

	int angle = ((int)((_theta+M_PI/4)/M_PI*180)*10)%3600;
	angle += 3600;
	angle %= 3600;

	_xSensorR = x + 0.006*cosLookup[angle];
	_ySensorR = y + 0.004*sinLookup[angle];

	angle = ((int)((_theta-M_PI/4)/M_PI*180)*10)%3600;
	angle += 3600;
	angle %= 3600;

	_xSensorL = x + (0.006)*cosLookup[angle];
	_ySensorL = y + (0.004)*sinLookup[angle];

	_theta = theta;
	_size = size;
	_velocity = velocity;
}

void Ant::move(int l)
{
	int angle = (((int)(_theta/M_PI*180))*10)%3600;

	_x += _velocity*cosLookup[(int)angle];
	_y += _velocity*sinLookup[(int)angle];

	angle = (((int)((_theta+M_PI/4)/M_PI*180))*10)%3600;
	angle += 3600;
	angle %= 3600;

	_xSensorR = _x + 0.006*cosLookup[(int)(angle)];
	_ySensorR = _y + 0.004*sinLookup[(int)(angle)];

	if(_xSensorR > 100000) cout << angle << endl;

	angle = (((int)((_theta-M_PI/4)/M_PI*180))*10)%3600;
	angle += 3600;
	angle %= 3600;
	
	_xSensorL = _x + 0.006*cosLookup[(int)(angle)];
	_ySensorL = _y + 0.004*sinLookup[(int)(angle)];
	
	
	
	// TODO solve with other solution
	//Border treatment
	if(_x < -0.98f || _x > 0.98f) 
	{
		_x = _x < 0 ? -0.98f : 0.98f;	
	} 

	if(_y < -0.98f || _y > 0.98f) 
	{
		_y = _y < 0 ? -0.98f : 0.98f;
	}

}

void Ant::environmentAnalysis(int viewFrequency, vector<int> &pheromoneMatrix)
{
	if(viewFrequency%50 == 0)
	{
		int rSensorPheromoneDetected = 0;
		int lSensorPheromoneDetected = 0;

		float xn, yn;

		xn = ((SCR_WIDTH/2) + _xSensorR * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorR * (SCR_HEIGHT/2));
		

		for(int i = -1; i <=1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				int actualColor = pheromoneMatrix[(((int)(yn+i))*SCR_WIDTH+(int)(xn+j))];

				bitset<32> r = (bitset<32>)actualColor & (bitset<32>)255;

	    		rSensorPheromoneDetected += (int)r.to_ulong() + (rand()%10)-5;
			}
		}

		xn = ((SCR_WIDTH/2) + _xSensorL * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorL * (SCR_HEIGHT/2));
		

		for(int i = -1; i <=1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				int actualColor = pheromoneMatrix[(((int)(yn+i))*SCR_WIDTH+(int)(xn+j))];

				bitset<32> r = (bitset<32>)actualColor & (bitset<32>)255;

	    		lSensorPheromoneDetected += (int)r.to_ulong() + (rand()%10)-5;
			}
		}
		
		if(rSensorPheromoneDetected > lSensorPheromoneDetected)_theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*1.3f;
		else if(rSensorPheromoneDetected < lSensorPheromoneDetected) _theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*1.3f;
		else if(_xSensorL < -0.98f || _xSensorL > 0.98f || _ySensorL < -0.98f || _ySensorL > 0.98f) _theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;
		else if(_xSensorR < -0.98f || _xSensorR > 0.98f || _ySensorR < -0.98f || _ySensorR > 0.98f) _theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;

		if(_theta < 0) _theta += 2*M_PI;
		if(_theta > 2*M_PI) _theta -= 2*M_PI;
	}		
}

void Ant::putPheromone(int* pheromoneMatrix)
{

}

void Ant::draw()
{

}

void Ant::run()
{

}
	
