/*
	Complete:
		Generate individual neural networks for each genome...
		Generate random connections between input and output layer...
		Each connection has weights

	TODO:
		1. Generate random nodes
		2. Display input values in real-time...
		3. Display connection values...
		4. Display everything...
*/

#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>	// Use for debugging
#include <vector>
#include <algorithm>

#include "Neuron.h"
#include "Connection.h"
#include "NET.h"
#include "Genome.h"

#define M 20
#define N 10


using namespace sf;

//
class Archive
{
public:

private:
	int populationSize = 0;
	int currentGeneration = 0;
	std::vector<Genome> elites;	// The top genomes
	std::vector<Genome> genomes;	// General Genomes
};

// Game values
int score = 0;
int level = 0;
int scorePerLVL = (level * 2) + 5;
int speed = 1;
bool aiMode = true;
int movesTaken = 0;
int moveLimit = 500;
int moveAlgorithm = {};
bool inspectMoveSelection = false;	// inspect move
float totalNumLines = 0.0;
float numLines = 0.0;
bool makeNewPopulation = false;

/*
	IDK WHAT VALUES THESE SHOULD BE RIGHT NOW
*/
// roundState
// saveState

// Evolutionary algorithm values
int populationSize = 10;
std::vector<Genome> genomes;
int currentGenome = 0;	// Start with this cuz we will be iterating
int generation = 0;
Archive globalArchive;
float mutationRate = 0.05;	// Similar to learning rate
float mutationStep = 0.2;	// Similar to momentum


bool goToNextShape = false;

// Initialize size of game field with 0's in all
int field[M][N] = { 0 };


// Each generation has a new genome
void createInitialPopulation(int numPopulation)
{
	
	for (int i = 0; i < numPopulation; i++)
	{	
		Genome *newGenome = new Genome();
		// Create new genome and push it to vector
		genomes.push_back(*newGenome);
		
	}

	//evaluateNextGenome();

}

// Mates the two parents to make a SUPER child, maybe, hopefully...please...
//Genome matingTime(Genome &Mother, Genome &Father)
//{
//
//}

/*
	Evaluates the next genome in the population.
	If there is none, evolves the population. Or if it reaches last genome
*/
void evaluateNextGenome()
{
	// Index for genome vector
	currentGenome++;

	// If there is none, evolves the population
	if (currentGenome == genomes.size())
	{
		// evolve();
	}

	// Load urrent gameState
	//loadState(roundState);

	// Reset moves taken
	movesTaken = 0;

	// and make the next move
	//makeNextMove();
}

// a for current Shape
// b = shape a one step behind
// c = next shape that comes up next
struct Point
{
	int x, y;
}a[4], b[4], c[4];




/*
	Figures follow this layout
	[0][1]
	[2][3]
	[4][5]
	[6][7]
*/
int figures[7][4] =
{
	{1,3,5,7},	// I 
	{2,4,5,7},	// Z 
	{3,5,4,6},	// S 
	{3,5,4,7},	// T 
	{2,3,5,7},	// L 
	{3,5,7,6},	// J 
	{2,3,4,5},	// O  = 6
};

bool check()
{
	for (int i = 0; i < 4; i++)
	{
		// Check if any blocks in the shape are out of bounds
		if (a[i].x < 0 ||	// Left boundary
			a[i].x >= N ||	// Right boundary
			a[i].y >= M		// Bottom boundary
			)
		{
			return false;
		}

		// Get the value of a specific position in the field
		// If it is 1 or 0 if there is an object there already
		else if (field[a[i].y][a[i].x])
		{
			return false;
		}

	}
	return true;
}


void resetGame()
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)

		{
			field[i][j] = 0;
		}
	}
	score = 0;
	numLines = 0.0;
	
	currentGenome++;	// Goto next genome

	if (currentGenome == populationSize)
	{
		// Set flag to create new population/generation
		makeNewPopulation = true;
	}
}



// Returns numLines cleared
float checkLines(int (&_field)[M][N])
{
	int k = M - 1;
	float numLines = 0.0;
	float points = 0.0;
	// Naive method
	for (int i = M - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < N; j++)
		{
			// Counts # of 1's at row
			if (_field[i][j])
			{
				count++;
			}

			// If any changes, this will shift everything down one.
			_field[k][j] = _field[i][j];
		}
		// If count != N
		if (count < N)
		{
			// Check next row
			k--;
		}

		// Increase numLines at cleared line
		if (count == N)
		{
			numLines += 1;
		}
	}

	return numLines;
}

float getFitness(float _score, float _insertRowsCleared,
	float _insertWeightedHeight,
	float _insertCumulativeHeight,
	float _insertRelativeHeight,
	float _insertHoles,
	float _insertRoughtness)
{
	return 2 * _score + 5 * _insertRowsCleared - 1.5 * _insertWeightedHeight - _insertCumulativeHeight - _insertRelativeHeight - _insertHoles - _insertRoughtness;
}


// Get all the holes with a block above until we reach to top of highest height
float checkHoles(int (&_field)[M][N])
{
	
	float sumOfHoles = 0.0;

	// Check until reaching row where no blocks reside
	// Start at very bottom
	for (int i = M-1; i > 0; i--)
	{
		int holesSeen = 0;
		int holesAtRow = 0;

		// Check every column at row i
		for (int j = 0; j < N; j++)
		{
			if (_field[i][j])
			{
				// If no block/hole is seen WITH a block above it
				continue;
			}

			if (_field[i][j] != 0 && _field[i - 1][j])
			{
				// If there is a hole with a block above
				holesSeen++;
			}
			holesAtRow++;
			
		}
		
		// Check if reached top of everything
		if (holesAtRow == N)
		{
			return sumOfHoles;
		}

		// If not reached top yet, sum holes
		sumOfHoles += holesSeen;
	}
}

//	Get the total sum of heights at current state of game
float getCumulativeHeight(int(&_field)[M][N])
{

	float totalSumofHeights = 0.0;
	
	for (int i = 0; i < N; i++)
	{

		float heightAtRow = 0.0;

		for (int j = M - 1; j > 0; j--)
		{
			// If there's a block, plus one
			if (_field[j][i])
			{
				heightAtRow++;
			}
			
		}

		totalSumofHeights += heightAtRow;
	}


	return totalSumofHeights;
	
}

// Get the highest height seen
float getWeightedHeight(int(&_field)[M][N])
{

	// Hold the height values
	std::vector<float> whatIsHighest;

	for (int i = 0; i < N; i++)
	{

		float heightAtRow = 0.0;

		for (int j = M - 1; j > 0; j--)
		{
			// If there's a block, plus one
			if (_field[j][i])
			{
				heightAtRow++;
			}

		}

		whatIsHighest.push_back(heightAtRow);
	}

	// Sort in ascending order
	std::sort(whatIsHighest.begin(), whatIsHighest.end());

	// return value at the end since ascending order
	return whatIsHighest[whatIsHighest.size() - 1];
}

// Get the absolute difference between the highest and lowest height
float getRelativeHeight(int(&_field)[M][N])
{

	// Hold the height values
	std::vector<float> whatIsHighest;

	for (int i = 0; i < N; i++)
	{

		float heightAtRow = 0.0;

		for (int j = M - 1; j > 0; j--)
		{
			// If there's a block, plus one
			if (_field[j][i])
			{
				heightAtRow++;
			}

		}

		whatIsHighest.push_back(heightAtRow);
	}

	// Sort in ascending order
	std::sort(whatIsHighest.begin(), whatIsHighest.end());
	

	// return value at the end since ascending order
	return abs(whatIsHighest[whatIsHighest.size() - 1] - whatIsHighest[0]);
}

// Get the roughness
// The sum of absolute difference between each column
float getRoughness(int(&_field)[M][N])
{
	float totalRoughness = 0.0;

	for (int i = 0; i < N; i++)
	{
		float current = 0.0;
		float next = 0.0;
		for (int j = M - 1; j > 0; j--)
		{
			
			if (_field[j][i])
			{
				current++;
			}
			if (_field[j][i + 1])
			{
				next++;
			}
			
		}

		totalRoughness += abs(current - next);
	}

	return totalRoughness;
}

int main()
{
	srand(time(NULL));

	// Create window object with size and name
	RenderWindow window(VideoMode(320, 480), "TheGame!");

	Font font;
	Text score_t;	// For score
	std::string score_s;
	Text speed_t;	// For displaying speed
	std::string speed_s;

	// Display current genome num
	Text genome_Text;
	std::string genome_s;

	Text generation_Text;
	std::string generation_s;

	Text fitness_Text;
	std::string fitness_s;


	if (!font.loadFromFile("font/XpressiveBlackRegular.ttf"))
	{
		std::cout << "Could not open font" << std::endl;
	}
	
	
	// Create Texture object
	Texture t;
	t.loadFromFile("images/tiles.png");

	std::string txt = "Shape";

	// Make image into sprite s
	Sprite currShape(t);
	Sprite nextShape(t);

	// Intrect: Define rectangle starting at (0,0) of sprite S with size 18x18 <- size of one block on image
	// Then set the texture for this rect
	currShape.setTextureRect(IntRect(0, 0, 18, 18));
	nextShape.setTextureRect(IntRect(0, 0, 18, 18));

	// Chose shape 3
	int n = rand() % 7;	// For shape a at initial
	int m = rand() % 7;	// For shape c at initial


	int dx = 0;	// Holder for x offset
	bool rotate = 0;
	int colorNum = 1;

	float timePassed = 0, gameSpeed = 0.3;

	Clock clock;

	// Get new block at very beginning
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}
	// Get next block at very beginning
	for (int i = 0; i < 4; i++)
	{
		c[i].x = figures[m][i] % 2;
		c[i].y = figures[m][i] / 2;
	}

	/*
		IDK WHAT RETURN VALUES THESE ARE
	*/
	// saveState = getState();
	// roundState = getState();

	createInitialPopulation(populationSize);

	

	while (window.isOpen())
	{
		Event e;
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timePassed += time;

		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::KeyPressed)
			{
				if (e.key.code == Keyboard::Escape)
				{
					window.close();
				}
				else if (e.key.code == Keyboard::Up)
				{
					rotate = 1;
				}
				else if (e.key.code == Keyboard::Left)
				{
					dx = -1;
				}
				else if (e.key.code == Keyboard::Right)
				{
					dx = 1;
				}
				else if (e.key.code == Keyboard::Down)
				{
					gameSpeed = gameSpeed/5;
					score = score + (2 * scorePerLVL);
					
				}
				else if (e.key.code == Keyboard::U)
				{
					speed = speed * 2;
				}
				else if (e.key.code == Keyboard::J)
				{
					if (speed == 1)
					{
						speed = 1;
					}
					else
					{
						speed = speed / 2;
					}
				}
			}

		}

		// Check current genomes
		if (makeNewPopulation)
		{
			makeNewPopulation = false;
			createInitialPopulation(populationSize);
		}

		////// <- Move -> ///////
		for (int i = 0; i < 4; i++)
		{
			b[i] = a[i];	// B shape is now A shape
			a[i].x += dx;	// x Offset if any
		}
		// Check if A shape is going off bounds
		if (!check())
		{
			for (int i = 0; i < 4; i++)
			{
				a[i] = b[i];	// A shape is now shape B before going out of bounds
			}
		}

		// Reset game speed
		if (speed == 256)
		{
			speed = 1;
		}

		/*for (int i = 0; i < 4; i++)
		{
			std::cout << "a[" << i << "].x = " << a[i].x;
			std::cout << " a[" << i << "].y = " << a[i].y << " Shape: " << n <<  std::endl;
		}*/

		//// Rotate ////
		if (rotate)
		{
			Point p = a[1];	// Center of rotation
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			// Check if new rotation is out of bounds
			if (!check())
			{
				for (int i = 0; i < 4; i++)
				{
					// If rotating it makes it go out of bounds,
					// force it be the orientation before rotating
					a[i] = b[i];
				}
			}
			
		}

		//// Tick ////
		if (timePassed > gameSpeed)
		{
			// Drop the shape
			for (int i = 0; i < 4; i++)
			{
				b[i] = a[i];
				// Go through each block of shape and update its positions
				// to go down one block
				a[i].y += 1;

				std::cout << "Genome: " << genomes[currentGenome].getID() << std::endl;
				std::cout << "Output[0]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][0]._outputVal << std::endl;
				std::cout << "Output[1]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][1]._outputVal << std::endl;
				std::cout << "Output[2]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][2]._outputVal << std::endl;
				std::cout << "Output[3]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][3]._outputVal << std::endl;
			}

			// Check if reached bottom boundary
			if (!check())
			{
				// Save the shape on the field
				for (int i = 0; i < 4; i++)
				{
					field[b[i].y][b[i].x] = colorNum;

				}
				colorNum = 1 + rand() % 7;

				n = rand() % 7;

				// Take next block
				for (int i = 0; i < 4; i++)
				{
					a[i] = c[i];
				}

				// Generate New next block
				for (int i = 0; i < 4; i++)
				{
					c[i].x = figures[n][i] % 2;
					c[i].y = figures[n][i] / 2;
				}


				for (int i = 0; i < M; i++)
				{
					for (int j = 0; j < N; j++)
					{
						std::cout << "[" << field[i][j] << "]";
					}
					std::cout << std::endl;
				}
				std::cout << std::endl << std::endl;
				std::cout << "Genome: " << genomes[currentGenome].getID() << std::endl;
				std::cout << "Output[0]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][0]._outputVal << std::endl;
				std::cout << "Output[1]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][1]._outputVal << std::endl;
				std::cout << "Output[2]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][2]._outputVal << std::endl;
				std::cout << "Output[3]: " << genomes[currentGenome].thisGenomeNet->m_Layers[1][3]._outputVal << std::endl;



			}
			
			// Reset timePassed
			timePassed = 0;
		}


		numLines += checkLines(field);
		totalNumLines += numLines;	// Get toatl NumLines
		// Check for lines cleared if any
		score += numLines* 15 * scorePerLVL;
		numLines = 0.0;	// Reset after each clear

		// Reset change from position
		dx = 0; 
		// Reset rotate or not
		rotate = 0; 
		// Reset time gameSpeed (seconds)
		gameSpeed = 0.3 / speed;

		
		//// DRAW ////
		window.clear(Color::White);

		// At every field,
		// Check if there is a block
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (field[i][j] == 0)
				{
					// No blocks present
					continue;
				}
				currShape.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));

				// This is like saving previous block
				currShape.setPosition(j * 18, i * 18);
				window.draw(currShape);
			}
		}

		// Update scores text
		score_s = std::string("Score ") + std::to_string(score).c_str();
		score_t.setFont(font);
		score_t.setString(score_s.c_str());
		score_t.setCharacterSize(20);
		score_t.setPosition(60, 50);
		score_t.setColor(Color::Red);
		window.draw(score_t);

		// Display speed and update
		speed_s = std::string("Speed: x") + std::to_string(speed).c_str();
		speed_t.setFont(font);
		speed_t.setString(speed_s.c_str());
		speed_t.setCharacterSize(20);
		speed_t.setPosition(200, 50);
		speed_t.setColor(Color::Red);
		window.draw(speed_t);

		// Display generation
		generation_s = std::string("Generation: ") + std::to_string(generation);
		generation_Text.setFont(font);
		generation_Text.setString(generation_s.c_str());
		generation_Text.setCharacterSize(20);
		generation_Text.setPosition(20, 380);
		generation_Text.setColor(Color::Red);
		window.draw(generation_Text);

		// Display Genome #
		genome_s = std::string("Genome[") + std::to_string(currentGenome) + std::string("]: ") + std::to_string(genomes[currentGenome].getID());
		genome_Text.setFont(font);
		genome_Text.setString(genome_s.c_str());
		genome_Text.setCharacterSize(20);
		genome_Text.setPosition(20, 400);
		genome_Text.setColor(Color::Red);
		window.draw(genome_Text);


		// Display Fitness
		fitness_s = std::string("Fitness: ") + std::to_string(getFitness(score, 
																totalNumLines, 
																getWeightedHeight(field), 
																getCumulativeHeight(field), 
																getRelativeHeight(field), 
																checkHoles(field), 
																getRoughness(field)));
		fitness_Text.setFont(font);
		fitness_Text.setString(fitness_s.c_str());
		fitness_Text.setCharacterSize(20);
		fitness_Text.setPosition(20, 420);
		fitness_Text.setColor(Color::Red);
		window.draw(fitness_Text);

		// Display inuts...
		std::string input1 = std::string("Input[0]: ") + std::to_string(score);



		genomes[currentGenome].updateFitness(getFitness(score,
												totalNumLines,
												getWeightedHeight(field),
												getCumulativeHeight(field),
												getRelativeHeight(field),
												checkHoles(field),
												getRoughness(field)));


		// Genome takes current values from game, and updates NN
		genomes[currentGenome].setValues(score, 
							totalNumLines, 
							getWeightedHeight(field),
							getCumulativeHeight(field),
							getRelativeHeight(field),
							checkHoles(field),
							getRoughness(field));

		// Draw sprite at current location
		for (int i = 0; i < 4; i++)
		{
			// Get that little block from image
			// colorNum = Starting from which position of image for color
			currShape.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			nextShape.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));

			// Check if current spot already has a block
			// Game over
			if (field[a[i].y][a[i].x])
			{
				resetGame();
			}

			// Set position of sprite
			currShape.setPosition(a[i].x * 18, a[i].y * 18);
			nextShape.setPosition(c[i].x * 18 + 200, c[i].y * 18 + 250);
			window.draw(currShape);
			window.draw(nextShape);
			
		}
		
		window.display();
	}
	return 0;
}