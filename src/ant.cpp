#include <antColony.h>
#include <ant.h>


/*
estado 1 - Exploratório: coloca vermelho (evaporação média) foge vermelho
|
|	encontrou feromonio verde -> Estado 4 - formiga achou o verde!: coloca feromonio (qual?) procura verde
|										achou comida -> Vai pro estado 3 180
|
|	encontrou comida
|
|
estado 2 - Achou comida: coloca verde (evaporação lenta) e procura vermelho 
|
|
|	encontrou ninho com comida 
|
|
estado 3 - encontrou ninho: coloca verde (evaporação lenta e maior quantidade) e procura verde    
*/

Ant::Ant(float x, float y, float theta, float size, float velocity, float placedPheromoneIntensity, int pheromoneType)
{
	_x = x;
	_y = y;

	_xSensorR =0;
	_ySensorR = 0;

	_xSensorL = 0;
	_ySensorL = 0;

	_theta = theta;
	_size = size;
	_velocity = velocity;
	_pheromoneType = pheromoneType;
}

void Ant::move(int l)
{
	int angle = (int)((_theta/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	_x += _velocity*cosLookup[angle];
	_y += _velocity*sinLookup[angle];

	angle = (int)(((_theta+M_PI/4)/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;

	_xSensorR = _x + 0.002*cosLookup[angle];
	_ySensorR = _y + 0.002*sinLookup[angle];


	angle = (int)(((_theta-M_PI/4)/M_PI)*1800);
	if(angle < 0) angle += 3600;
	if(angle >= 3600) angle -= 3600;
	
	_xSensorL = _x + 0.002*cosLookup[angle];
	_ySensorL = _y + 0.002*sinLookup[angle];
	
	
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

void Ant::environmentAnalysis(int viewFrequency, vector<uint8_t> &pheromoneMatrix)
{
	if(viewFrequency%5 == 0)
	{
		int rSensorRedPheromoneDetected = 0;
		int lSensorRedPheromoneDetected = 0;
		int rSensorBluePheromoneDetected = 0;
		int lSensorBluePheromoneDetected = 0;

		int xn, yn, index;

		xn = ((SCR_WIDTH/2) + _xSensorR * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorR * (SCR_HEIGHT/2));


		for(int i = -1; i <=1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				index = ((yn+i) * SCR_WIDTH + (xn+j)) * 4;
				rSensorRedPheromoneDetected += pheromoneMatrix[index];
	    		rSensorBluePheromoneDetected += pheromoneMatrix[index+2];
			}
		}

		rSensorRedPheromoneDetected +=  (rand()%10)-5;
		rSensorBluePheromoneDetected +=  (rand()%10)-5;

		xn = ((SCR_WIDTH/2) + _xSensorL * (SCR_WIDTH/2));
		yn = ((SCR_HEIGHT/2) + _ySensorL * (SCR_HEIGHT/2));
		

		for(int i = -1; i <=1; i++)
		{
			for(int j = -1; j <= 1; j++)
			{
				index = ((yn+i) * SCR_WIDTH + (xn+j)) * 4;
				lSensorRedPheromoneDetected += pheromoneMatrix[index];
	    		lSensorBluePheromoneDetected += pheromoneMatrix[index+2];
			}
		}
		
		lSensorRedPheromoneDetected +=  (rand()%10)-5;
		lSensorBluePheromoneDetected +=  (rand()%10)-5;

		if(_pheromoneType == 1)
		{
			if(rSensorRedPheromoneDetected > lSensorRedPheromoneDetected)_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rSensorRedPheromoneDetected < lSensorRedPheromoneDetected) _theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			if(rSensorBluePheromoneDetected > lSensorBluePheromoneDetected)_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rSensorBluePheromoneDetected < lSensorBluePheromoneDetected) _theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;

			if(_x >= -0.01f && _x <= 0.01f && _y >= 0.3f && _y<= 0.31f) _pheromoneType = 2;
		}
		else
		{
			if(rSensorRedPheromoneDetected > lSensorRedPheromoneDetected)_theta += glm::radians((float)(rand()%360)/6.0f)*0.4f;
			else if(rSensorRedPheromoneDetected < lSensorRedPheromoneDetected) _theta -= glm::radians((float)(rand()%360)/6.0f)*0.4f;
			//if(rSensorBluePheromoneDetected > lSensorBluePheromoneDetected)_theta += glm::radians((float)(rand()%360)/6.0f)*0.5f;
			//else if(rSensorBluePheromoneDetected < lSensorBluePheromoneDetected) _theta -= glm::radians((float)(rand()%360)/6.0f)*0.5f;
		}

		

		if(_xSensorL < -0.98f || _xSensorL > 0.98f || _ySensorL < -0.98f || _ySensorL > 0.98f) _theta += glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;
		else if(_xSensorR < -0.98f || _xSensorR > 0.98f || _ySensorR < -0.98f || _ySensorR > 0.98f) _theta -= glm::radians((float)(rand()%360)/10.0f-1.0f)*4.0f;

		if(_theta < 0) _theta += 2*M_PI;
		if(_theta >= 2*M_PI) _theta -= 2*M_PI;
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
	
