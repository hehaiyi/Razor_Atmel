/**********************************************************************************************************************
File: user_app1.c                                                                
He Haiyi
----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */
extern u8 G_u8DebugScanfCharCount;
extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
//static u8 au8Myname[5];
//static u8 u8CorsurPosition=0;

void UserApp1Initialize(void)
{
 // LCDCommand(LCD_HOME_CMD);  
 // u8Corcurposition=LINE1_START_ADDR;
 /* u8 au8Message[]="hellow world";
  LCDMessage(LINE1_START_ADDR,au8Message);
  LCDClearChars(LINE1_START_ADDR+5,7);
  LCDMessage(LINE2_START_ADDR,"|");
  LCDMessage(LINE2_END_ADDR,"|");
  LCDMessage(LINE2_START_ADDR+8,"EiE");
  LCDCommand(LCD_DISPLAY_CMD);
  LCDCommand(LCD_DISPLAY_CMD|LCD_DISPLAY_ON);
  LCDCommand(LCD_DISPLAY_CMD|LCD_DISPLAY_ON|LCD_DISPLAY_CURSOR);
  LCDCommand(LCD_DISPLAY_CMD|LCD_DISPLAY_ON|LCD_DISPLAY_CURSOR|LCD_DISPLAY_BLINK);
  LCDCommand(LCD_HOME_CMD);
  LCDCommand(LCD_ADDRESS_CMD|3);
  */
  LCDClearChars(0,20);
  /* If good initialization, set state to Idle */
  if( 1 )
  {    
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{ 
  
    static u8 auArray[9]={' ','h','e','h','a','i','y','i',' '};
    static bool bRightOrLeft=TRUE;
    static u8 u8StepOfName=11;
    static u16 u16MovementTntervalTime=0;
    u16MovementTntervalTime++;
    if(u16MovementTntervalTime==500)
    { u16MovementTntervalTime=0;
      if(bRightOrLeft)
      {
        LCDMessage(u8StepOfName,auArray);
      }
      else
      {
        LCDMessage(LINE1_END_ADDR-u8StepOfName-8,auArray);
      }
      u8StepOfName--;
      if(u8StepOfName==0)
      {
       u8StepOfName=11;
       bRightOrLeft=!bRightOrLeft;
      }
    }
  
    
   /* 
    static u8 auArray[20]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','h','e','h','a','i','y','i',' '};
    static u8 u8StepOfName=13;
    static u16 u16MovementTntervalTime=0;
    u16MovementTntervalTime++;
    if(u16MovementTntervalTime==500)
    { u16MovementTntervalTime=0;
      LCDMessage(LINE1_START_ADDR,auArray);
      for(u8 i=0;i<19;i++)
      {
         auArray[i]=auArray[i+1];
         
      }
      //u8StepOfName--;
      auArray[19]=auArray[0];
      
    
     
      
   //  LCDMessage(LINE1_END_ADDR-u8StepOfName-8,auArray);
     
  //    u8StepOfName--;
  //    if(u8StepOfName==0)
 //     {
  //     u8StepOfName=11;
   //   }
    }
  
  
 */
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /* 
  static u8 u8CorsurPosition=0;
  static u8 au8Myname[]="hehaiyi";
  static u8 u8Position=0;
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LCDCommand(LCD_HOME_CMD);
    LCDCommand(LCD_DISPLAY_CMD|LCD_DISPLAY_ON|LCD_DISPLAY_CURSOR|LCD_DISPLAY_BLINK);
  
  }
    if(G_u8DebugScanfCharCount==1)
    {  
     DebugScanf(au8Myname);
     LCDMessage(LINE1_START_ADDR+u8CorsurPosition%10,au8Myname);
     u8Position++;
    }
  
  
  if(WasButtonPressed(BUTTON3))
   {
     ButtonAcknowledge(BUTTON3);
     
     if(u8CorsurPosition==LINE1_END_ADDR)
     {
        u8CorsurPosition=LINE2_START_ADDR;
        
     }
   
     else if(u8CorsurPosition==LINE2_END_ADDR)
     {
        u8CorsurPosition=LINE1_START_ADDR;
       
     }
     else
     {
        u8CorsurPosition++;
     }
    
    LCDCommand(LCD_ADDRESS_CMD|u8CorsurPosition);
   }
     
  if(WasButtonPressed(BUTTON2))
  {
     ButtonAcknowledge(BUTTON2);
     
     if(u8CorsurPosition==LINE1_START_ADDR)
     {
       u8CorsurPosition=LINE2_END_ADDR;
     }
     else if(u8CorsurPosition==LINE2_START_ADDR)
     {
        u8CorsurPosition=LINE1_END_ADDR;
     }
     else
     {
        u8CorsurPosition--;
     }
  LCDCommand(LCD_ADDRESS_CMD|u8CorsurPosition);
  }
     
  
      
       if(WasButtonPressed(BUTTON1))
    {
        ButtonAcknowledge(BUTTON1);
        if(u8CorsurPosition<10)
        {
           u8CorsurPosition=LINE2_START_ADDR+u8CorsurPosition%10;
        }
        if(u8CorsurPosition>=10)
        {
           u8CorsurPosition=LINE1_START_ADDR+u8CorsurPosition%10;
        }
   
    }
 
*/
     
 
 
 
 
 
 
 
 
       
     
     
     
  
} /* end UserApp1SM_Idle() */
    
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
