#include "EasyBMP.cpp"
#include "EasyBMP.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//default size of the queue
const int MSIZE = 1000;

// allows for a pixels x and y values to be 
//declared as a point
class point {
private:
  int x; int y;

public:

  //declares the point and assigns
  //x and y values
  point(int p, int q) {
    x = p; y = q;
  }

  int getx() {
    return x;
  }

  int gety() {
    return y;
  }
};

//allows for a queue to be inistialized
// making it easy to perform specific tasks
//such as insert and delete
class queue {

private:
  point* Q[MSIZE];

  int front, rear, size;

public:
  queue() {
    // initialize an empty queue
    front = 0; rear = 0; size = 0;
    for (int j=0; j < MSIZE; ++j)
      Q[j] = 0;
  }

  void insert(point* x) {
    if (size != MSIZE) {
      front++; size++;
      if (front == MSIZE) front = 0;
      Q[front] = x;
    }
  }

  point del() {
    if (size != 0) {
      rear++; if (rear == MSIZE) rear = 0;
      point temp(Q[rear]->getx(), Q[rear]->gety());
      size--;
      return temp;
    };
  }

  void print() {
    for (int j = 1; j <= size; ++j) {
      int i = front - j + 1;
      cout << "x = " << Q[i]->getx() << " y = " << Q[i]->gety() << endl;
    }
    cout << "end of queue" << endl;
  }

  bool isEmpty() {
    return (size == 0);
  }
};


int Conversion(BMP & imageIn, BMP & imageOut)
{
  
  //RGB color declaration of green and red
  /***************/
  RGBApixel red;
  red.Red = 255;
  red.Green = 0;
  red.Blue = 0;
  /**************/
  RGBApixel green;
  green.Red = 0;
  green.Green = 255;
  green.Blue = 0;
  /*************/

  // declaration of variables to count
  //amount of lines and letters in the image
  int letterCount = 0;
  int lineCount = 0;

  //Declares the height and width of image
  int width = imageIn.TellWidth();
  int height = imageIn.TellHeight();

  imageOut.SetSize(width, height);

  //boolean 2d-array to verify if a pixel has
  // been visited or not
  bool visited[width][height];

  //inistializes the array setting all values equal
  //to false
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      visited[j][i] = false;

  //x and y high and low values of a pixel
  int xlow = 0;
  int xhigh = width;

  int ylow = 0;
  int yhigh = height;

  // Declares a value for the top of a line an
  // bottom of a line
  int lineBottom = 0;
  int lineTop = 0;

  queue Q;

  // Nested loops to loop over every pixel in the image
  for (int j = 0; j < height; j++){// Start for1
    for (int k = 0; k < width; k++){// Start for 2
      if (visited[k][j] == true)
	continue;
      //Checks to see if the pixel is black
      if (imageIn(k, j)->Red >= 20){
	visited[k][j] = true;
	continue;
      }
      
      letterCount++;

      xlow = k;
      ylow = j;
      xhigh = k;
      yhigh = j;

      Q.insert(new point(k, j));

      int currentTop = ylow;
      int currentBottom = yhigh;

      visited[k][j] = true;

      while(!Q.isEmpty()){// Start while
	//gets point value of a pixel
	point p = Q.del();
	int x = p.getx();
	int y = p.gety();

	imageOut.SetPixel(x, y, green);

	if(x < xlow)
	  xlow = x;
	if(y < ylow)
	  ylow = y;
	if (x > xhigh)
	  xhigh = x;
	if (y > yhigh)
	  yhigh = y;

	//finds the neighbors of a pixel by
	// finding the pixels around it that are black
	for (int l = y - 1; l <= y + 1; l++){// Start for3
	  for (int m = x - 1; m <= x + 1; m++){// Start for 4
	    if (visited[m][l] == false){// Start if1
	      visited [m][l] = true;
	      if (imageIn(m, l)->Red < 20)
		Q.insert(new point(m, l));
	    }// End if1
	  }// End for4
	}// End for 3
      }// End while

      //draws bounding boxes around each letter
      for (int i = xlow - 1; i <= xhigh + 1; i++){
	imageOut.SetPixel(i, ylow - 1, red);
	imageOut.SetPixel(i, yhigh + 1, red);
      }

      for (int j = ylow - 1; j <= yhigh + 1; j++){
	imageOut.SetPixel(xlow - 1, j, red);
	imageOut.SetPixel(xhigh + 1, j, red);
      }

      //finds the number of lines in the image
      if (currentTop > lineBottom){
	lineCount++;
	lineTop = currentTop;
	lineBottom = currentBottom;
      }
    }// End for2
  }// End for1

  cout << "Letter Count: " << letterCount << endl;
  cout << "Line Count: " << lineCount << endl;

}


 
int main (int argc, char* argv[])
{
  BMP image;
  image.ReadFromFile(argv[1]);

  int width = image.TellWidth();
  int height = image.TellHeight();

  BMP imageOut;
  imageOut.SetSize(width, height);

  Conversion(image, imageOut);

  imageOut.WriteToFile(argv[2]);
  return 0;
}
