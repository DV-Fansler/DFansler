#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>    // read/write usleep     
#include <stdlib.h>    // exit function
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <sys/ioctl.h>			//Needed for I2C port
#include <math.h>
#include "/home/pi/glg/include/GlgApi.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "BSP.h"
#include <stdbool.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950
#endif

/* Function prototypes */
void Input( GlgObject viewport, GlgAnyType client_data, GlgAnyType call_data );
void InitializeDrawing( GlgObject viewport );
void UpdateDrawing( GlgAnyType client_data, GlgLong *timer_id );
void SetSize( GlgObject viewport, GlgLong x, GlgLong y, 
              GlgLong width, GlgLong height );
void StopUpdates( void );
void StartUpdates( void );
double GetData( double low, double high );
double Intensity;
#define  UPDATE_INTERVAL     100     /* Update interval, msec. */


/* If set to true, tags defined in the drawing are used for animation.
   Otherwise, object resources are used to push real-time values into the
   drawing.
*/
#define USE_TAGS False

GlgObject Drawing;
GlgAppContext AppContext;

GlgLong TimerID;

/* Defines a platform-specific program entry point */
#include "GlgMain.h"

/*----------------------------------------------------------------------*/
int GlgMain( argc, argv, InitAppContext )
     int argc;
     char *argv[];
     GlgAppContext InitAppContext;
{

   AppContext = GlgInit( False, InitAppContext, argc, argv );

   /* Load a drawing from the file. */
   Drawing = GlgLoadWidgetFromFile( "Lesson_3.g" );

   if( !Drawing )
     exit( GLG_EXIT_ERROR );
   
   /* Set window size. */
   SetSize( Drawing, 0, 0, 800, 600 );

   /* Setting the window name (title). */
   GlgSetSResource( Drawing, "ScreenName", "Lesson 3 & 4 Example" );

   /* Add Input callback to handle user interaction in a GLG input widget
      such as a button or a slider.
   */
   GlgAddCallback( Drawing, GLG_INPUT_CB, Input, NULL );

   /* Set initial values for GLG widgets. */
   InitializeDrawing( Drawing );

   /* Paint the drawing. */   
   GlgInitialDraw( Drawing );

   /* Start periodic dynamic updates. */
   StartUpdates();
   
   return (int) GlgMainLoop( AppContext );
}

/*----------------------------------------------------------------------
| Set initial parameters for GLG objects/widgets.
*/
void InitializeDrawing( GlgObject viewport )
{
   /* Set initial patameters as needed. */
   Run_Value = 0;
}

/*----------------------------------------------------------------------
| Periodic dynamic updates.
*/
void UpdateDrawing( GlgAnyType client_data, GlgLong * timer_id )
{
   /* Obtain simulated demo data values in a specified range.
      The application should provide a custom implementation
      of the data acquisition interface to obtain real-time
      data values.
   */

   
   if( USE_TAGS ) /* Use tags for animation. */
   {
      /* Push values to the objects using tags defined in the drawing. 
      GlgSetDTag( Drawing, "Voltage", voltage, GlgTrue //if_changed );
      GlgSetDTag( Drawing, "Current", current, GlgTrue //if_changed );*/
      ;
   }
   else if(Run_Value == 0)/* Use resources for animation. */
   {
       if(DEBUG1 == 1)
         printf("call adc\n");
          /* Push values to the objects using resource paths. */
      ADC_Read(0);      //read chanel 0 of ADS1115
      Intensity = ADC_Value;
      GlgSetDResource( Drawing, "LampIntensity/Value", Intensity );
   
      if(DEBUG == 1)
         printf("ADC_Read  = %.1f\n",Intensity);
   }
   GlgUpdate( Drawing );    /* Make changes visible. */
   
   /* Reinstall the timer to continue updating */
   TimerID = GlgAddTimeOut( AppContext, UPDATE_INTERVAL, 
                          (GlgTimerProc) UpdateDrawing, (GlgAnyType) NULL );
}

/*----------------------------------------------------------------------
| Start periodic dynamic updates.
*/
void StartUpdates()
{
   TimerID = GlgAddTimeOut( AppContext, UPDATE_INTERVAL,
                            (GlgTimerProc) UpdateDrawing, 
                            (GlgAnyType) NULL );
}

/*----------------------------------------------------------------------
| Stop dynamic updates.
*/
void StopUpdates()
{
   /* Stop update timers */
   if( TimerID )
   {
      GlgRemoveTimeOut( TimerID );
      TimerID = 0;
   }
}

/*----------------------------------------------------------------------
| This callback is invoked when user interacts with input objects in GLG
| drawing, such as a slider, dial or a button.
*/
void Input( GlgObject viewport, GlgAnyType client_data, GlgAnyType call_data )
{
   GlgObject message_obj;
   char
     * format,
     * action,
     * origin;      

   message_obj = (GlgObject) call_data;

   /* Get the message's format, action and origin. */
   GlgGetSResource( message_obj, "Format", &format );
   GlgGetSResource( message_obj, "Action", &action );
   GlgGetSResource( message_obj, "Origin", &origin );
   if(DEBUG1 == 1)
   {
      printf("&format = %s\n",format);
      printf("&action = %s\n",action);
      printf("Origin = %s\n",origin);
   }
 //   Handle window closing. May use viewport's name. 
   if( strcmp( format, "Window" ) == 0 && strcmp( action, "DeleteWindow" ) == 0 )
   
       exit( GLG_EXIT_OK );

   /* Input event occurred in a button. */ 
   if( strcmp( format, "Button" ) == 0 )
   {
            if(DEBUG == 1) 
               printf("Button == 0\n");
  if( strcmp( action, "Activate" ) == 0 ) /* Push button events. */
      {
              if(DEBUG1 == 1) 
               printf("Activate == 0\n");
      /* The user selected a Quit button: exit the program. */
             if(DEBUG == 1)
               printf("QuitButton origin\n");  
         if( strcmp( origin , "QuitButton" ) == 0 )
         {
            if(DEBUG == 1)
               printf("QuitButton\n");
           exit( GLG_EXIT_OK );
         }
      }
      else if( strcmp( action, "ValueChanged" ) == 0 ) /* Toggle button events. */
      {
         if( strcmp( origin, "Lamp" ) == 0 )
         {
            if(DEBUG == 1)
               printf("Lamp == 0\n");
            double value;
            GlgGetDResource( message_obj, "OnState", &value );
            if(DEBUG == True)
               printf("OnState value = %f, \n",value );
            switch( (int)value)
            {
               case 0:
                  if(DEBUG == 1)
                     printf("Stop updates\n");
                  Run_Value = 0;
                  StopUpdates();   // Stop updates. 
                  break;
               case 1:
                  if(DEBUG == 1)
                     printf("Start updates\n");
                  Run_Value = 1;                    
                  StartUpdates();  // Start updates. 
                  break;
            }
         }
      }
      GlgUpdate( viewport );  

   }
       GlgUpdate( viewport );  
       if(DEBUG1 == 1)
           printf("exit Input\n");
   
}

/*----------------------------------------------------------------------
| Set viewport size in screen cooridnates. 
*/
void SetSize( GlgObject viewport, GlgLong x, GlgLong y, 
              GlgLong width, GlgLong height )
{
   GlgSetGResource( viewport, "Point1", 0., 0., 0. );
   GlgSetGResource( viewport, "Point2", 0., 0., 0. );

   GlgSetDResource( viewport, "Screen/XHint", (double) x );
   GlgSetDResource( viewport, "Screen/YHint", (double) y );
   GlgSetDResource( viewport, "Screen/WidthHint", (double) width );
   GlgSetDResource( viewport, "Screen/HeightHint", (double) height );
}

/*----------------------------------------------------------------------
| Generates demo data value within a specified range. 
| An application can replace code in this function to supply 
| real-time data from a custom data source.
*/
double GetData( double low, double high )
{
   double 
     half_amplitude, center,
     period,
     value,
     alpha;
   
   static int counter = 0;

   half_amplitude = ( high - low ) / 2.0;
   center = low + half_amplitude;
   
   period = 100.0;
   alpha = 2.0 * M_PI * counter / period;
   
   value = center +
     half_amplitude * sin( alpha ) * sin( alpha / 30.0 );
   
   ++counter;
   return value;
}

void delay(int dtime)
{
   int dt,dt_main;
   for (dt_main=0;dt_main <dtime;dt_main++)
   {
       for(dt = 0;dt<1000;dt++)
           ;
   }
   return;
}
