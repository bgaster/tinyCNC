/*
 * Source: tinyCNC.ino
 * Author: Benedict R. Gaster
 */

#include <Servo.h>
#include <PString.h>

// DEBUG
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
PString message(buffer, BUFFER_SIZE);

struct point
{
    unsigned int x_;
    unsigned int y_;

point() :
    x_(0), y_(0)
    {
    }
    

point(float x, float y) :
    x_(x), y_(y)
    {
    }
    
};

namespace {
    
constexpr unsigned int pin_servo_x{44};
constexpr unsigned int pin_servo_y{45};
constexpr unsigned int pin_servo_z{46};

// Coordiate System is x = [0,180) and y = [0,180) with (0,0) top left corner

};

class Pen
{
private:    
    
    static constexpr unsigned int pen_up_pos_{1};
    static constexpr unsigned int pen_down_pos_{70};
    static constexpr unsigned int step_delay_{15};

    Servo servo_x_;
    Servo servo_y_;
    Servo servo_z_;

    point pen_pos_;

    bool pen_down_;

    void swap ( float& a, float& b )
    {
	float c(a); a=b; b=c;
    }
public:
    Pen() :
       pen_pos_{1,1},
       pen_down_{false} 
    {
    }
    
    void attach(unsigned int pin_x, unsigned int pin_y, unsigned int pin_z)
    {
	servo_x_.attach( pin_x );
	servo_y_.attach( pin_y );
	servo_z_.attach( pin_z );

	up();
    }
    
    void down()
    {
	servo_z_.write(pen_down_pos_);
	delay( 1500 ); 
    }

    void up()
    {
	servo_z_.write(pen_up_pos_); 
	delay( 1500 );
    }
    
    void move(unsigned int x, unsigned int y)
    {
	servo_x_.write(x);
	servo_y_.write(y);
    }

    void move(point p)
    {
	servo_x_.write(p.x_);
	servo_y_.write(p.y_);
    }

    void step_delay()
    {
	delay(step_delay_);	
    }

    /*********************************a
     * Draw a line from (x0;y0) to (x1;y1). 
     * Bresenham algorythm from 
     *    http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm
     * int (x1;y1) : Starting coordinates
     * int (x2;y2) : Ending coordinates
     **********************************/
#if 0
    void draw_line( 
	float x1, 
	float y1, 
	float x2, 
	float y2)
    {
	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep) {
	    swap(x1, y1);
	    swap(x2, y2);
	}
	
	if(x1 > x2) {
	    swap(x1, x2);
	    swap(y1, y2);
	}
	
	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);
	
	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;
	
	const int maxX = (int)x2;
	
	for(int x=(int)x1; x<maxX; x++) {
	    point p;
	    
	    if(steep) {
		p = point(y,x);
		
	    }
	    else {
		p = point(x,y);
	    }
	    
	    move(p);
	    
	    error -= dy;
	    if(error < 0){
		y += ystep;
		error += dx;
	    }
	    
	    step_delay();
	}
    }
#else
void draw_line(int x0, int y0, int x1, int y1) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;){
      move(point{x0, y0});
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
    // Serial.print( "X=" ); Serial.print( x0 ); Serial.print( "  -  Y=" ); Serial.println( y0 );
    step_delay();           //delay for settling
  }
}
#endif
};

class Message
{
private:
    // Currently support only three messages
    //    pen_up
    //    pen_down
    //    drawline x1 y1 x2 y2
    // All messages begin with a single byte indicating the command
    static constexpr uint8_t msg_pen_up_{0};    // lift pen up
    static constexpr uint8_t msg_pen_down_{1};  // place pen down
    static constexpr uint8_t msg_draw_line_{2}; // move to point{x,y}

    // Each message is sent as a single line of text, makign it easier to parse
    static constexpr unsigned int msg_buffer_length_{1024}; // should be way more than we need
    char msg_buffer_[msg_buffer_length_];
    unsigned int position_;

    void handle(Pen& pen)
    {
	// pen_up message
	if (strncmp(msg_buffer_, "pen_up", strlen("pen_up")) == 0) {
	    pen.up();
	}

	// pen_down message
	if (strncmp(msg_buffer_, "pen_down", strlen("pen_down")) == 0) {
	    pen.down();
	}

	// draw_line message
	if (strncmp(msg_buffer_, "draw_line", strlen("draw_line")) == 0) {
	    char * next_pos1;
	    char * next_pos2;
	    
	    float x1 = strtod(msg_buffer_ + strlen("draw_line") + 1, &next_pos1);
	    float y1 = strtod(next_pos1 + 1, &next_pos2);

	    float x2 = strtod(next_pos2 + 1, &next_pos1);
	    float y2 = strtod(next_pos1 + 1, nullptr);

	    message = "draw_line ";
	    message += x1;
	    message += " ";
	    message += y1;
	    message += " ";
	    message += x2;
	    message += " ";
	    message += y2;
	    Serial.println(message);

	    pen.draw_line(x1, y1, x2, y2);
	}
    }
public:
    Message() :
       position_{0}
    {
    }
    
    void attach()
    {
	Serial.begin(57600);
	while (!Serial) { 
	    ; // wait for serial port to connect. Needed for Leonardo only
	}
    }

    void process(Pen& pen)
    {
	char incoming;
	
	while (Serial.available() > 0) {
	    incoming = Serial.read();
	    if (incoming == '\n' || incoming == '\r') {
		msg_buffer_[position_] = '\0';
		handle(pen);
		position_ = 0; // reset for next message
		continue; 
	    }
	    else {
		msg_buffer_[position_] = incoming;
		position_++;
	    }
	}
    }
};

namespace {
    
Pen pen;
Message msg;

}; // namespace

void setup() {
    msg.attach();

    pen.attach(pin_servo_x, pin_servo_y, pin_servo_z);
    // pen starts away from the paper, i.e. up
    pen.up();
    // move to the origin
    pen.move(0,0);

    /* pen_move_down(); */

//    pen.down();
//    pen.draw_line(1, 1, 100, 100);
    
//    pen.up();
    
    pinMode(13, OUTPUT);
}

void loop() 
{
    msg.process(pen);
}
