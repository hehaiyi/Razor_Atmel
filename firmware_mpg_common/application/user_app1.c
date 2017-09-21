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

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[];
extern u8 G_u8DebugScanfCharCount;
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
static void UserAPP1_state1(void);
static void UserAPP1_state2(void);

void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  { 
      PWMAudioOff(BUZZER1); 
      LedOff(WHITE);
      LedOff(GREEN);
      LedOff(RED);
      LedOff(BLUE);
      LedOff(PURPLE);
      LedOff(CYAN);
      LedOff(YELLOW);
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
static bool bIfBuzzerOn=FALSE;                                   /*Turn on Buzzer or Turn off Buzzer*/
static u16 u16TimeCounter=0;                                     /*to judge Buzzer On or Off*/
static void UserApp1SM_Idle(void)
{
      static u8 au8Commend[1];
      static bool bState1=FALSE;                                 /*to make the state1 only run once*/
      static bool bState2=FALSE;                                 /*to make the state1 only run once*/
      
      u16TimeCounter++;
      DebugScanf(au8Commend);
      
      /*state1*/
      if(WasButtonPressed(BUTTON1))                             /*Button1 Control*/
      {
          ButtonAcknowledge(BUTTON1);
          UserApp1_StateMachine=UserAPP1_state1;
      }
      
      if(au8Commend[0]=='1')                                    /*Tera Control*/
      {   
        bState1=TRUE;                                           /*into the State1*/
      }
      
      if(au8Commend[0]=='\r'&&bState1)
      {
          bState1=FALSE;
          UserApp1_StateMachine=UserAPP1_state1;
      }
      
      /*state2*/
      if(WasButtonPressed(BUTTON2))                             /*Button2 Control*/
      {
          ButtonAcknowledge(BUTTON2);
          UserApp1_StateMachine=UserAPP1_state2;
      }
      
      if(au8Commend[0]=='2')                                    /*Tera Control*/
      {   
          bState2=TRUE;                                         /*into the State2*/
      }
      
      if(au8Commend[0]=='\r'&&bState2)
      {
          bState2=FALSE;
          UserApp1_StateMachine=UserAPP1_state2;
      }
  
      /*Function of Buzzer*/
      if(bIfBuzzerOn)
      {    
          PWMAudioSetFrequency(BUZZER1, 200);
          
          if(u16TimeCounter==10)
          {
              PWMAudioOff(BUZZER1);  
          }
          
          if(u16TimeCounter==1000)
          {  
              PWMAudioOn(BUZZER1);
              u16TimeCounter=0;
          }
      } 
   
    /*This is my first version without StateMachine*/
    /*  
      static u16 u16TimeCounter1=0;
      u8 au8StringState1[]="Entering state 1";
      u8 au8StringState2[]="Entering state 2";
      u8 au8State1Message[] = "STATE 1";
      u8 au8State2Message[] = "STATE 2";
      static u8 auArray[1];
      static bool bIfBuzzer=TRUE;     
      
      u16TimeCounter1++;
      DebugScanf(auArray);
      
      if(!bIfBuzzer)  
      {
          PWMAudioSetFrequency(BUZZER1, 200);
          if(u16TimeCounter1==100)
          {
              PWMAudioOff(BUZZER1);  
          }
          if(u16TimeCounter1==1000)
          {  
              PWMAudioOn(BUZZER1);
              u16TimeCounter1=0;
          }
      }
      
      
      if(WasButtonPressed(BUTTON2)||auArray[0]=='2')
      {
          u16TimeCounter1=0;
          bIfBuzzer=FALSE;
          
          ButtonAcknowledge(BUTTON2);  
          auArray[0]=0;     
          DebugPrintf(au8StringState2);
          DebugLineFeed();
                    
          LedBlink(GREEN,LED_1HZ);
          LedBlink(YELLOW,LED_2HZ);
          LedBlink(ORANGE,LED_4HZ);
          LedBlink(RED,LED_8HZ);
          LedOff(WHITE);
          LedOff(PURPLE);
          LedOff(BLUE);
          LedOff(CYAN);
          LedPWM(LCD_RED,LED_PWM_55);
          LedPWM(LCD_GREEN,LED_PWM_30);
          LedPWM(LCD_BLUE,LED_PWM_15);
          
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR+8,au8State2Message);
      }
      
      if(WasButtonPressed(BUTTON1)||auArray[0]=='1')
      {
          ButtonAcknowledge(BUTTON1);  
          auArray[0]=0; 
          DebugPrintf(au8StringState1);
          DebugLineFeed();

          LedOn(WHITE);
          LedOn(PURPLE);
          LedOn(BLUE);
          LedOn(CYAN);
          LedOff(GREEN);
          LedOff(YELLOW);
          LedOff(ORANGE);
          LedOff(RED);
          LedPWM(LCD_RED,LED_PWM_35);
          LedPWM(LCD_GREEN,LED_PWM_20);
          LedPWM(LCD_BLUE,LED_PWM_45);
          bIfBuzzer=TRUE;
          
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR+8,au8State1Message);
          
      }*/
} /* end UserApp1SM_Idle() */
    
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif



static void UserAPP1_state1(void)
{
          bIfBuzzerOn=FALSE;                                      /*Turn off the Buzzer*/
         
          DebugPrintf("Entering state 1");
          DebugLineFeed();
          
          /*function of Led*/
          LedOn(WHITE);
          LedOn(PURPLE);
          LedOn(BLUE);
          LedOn(CYAN);
          LedOff(GREEN);
          LedOff(YELLOW);
          LedOff(ORANGE);
          LedOff(RED);
          LedPWM(LCD_RED,LED_PWM_35);
          LedPWM(LCD_GREEN,LED_PWM_20);
          LedPWM(LCD_BLUE,LED_PWM_45);
          
          /*function of Lcd*/          
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR+7,"STATE 1");
          
          /*Back to the UserApp1SM_Idle()*/
          UserApp1_StateMachine=UserApp1SM_Idle;                
  
}



static void UserAPP1_state2(void)
{
          u16TimeCounter=0;                                        /*Restart the counter*/
          bIfBuzzerOn=TRUE;                                        /*Turn on the buzzer*/ 
               
          DebugPrintf("Entering state 2");
          DebugLineFeed();
           
          /*function of Led*/
          LedBlink(GREEN,LED_1HZ);
          LedBlink(YELLOW,LED_2HZ);
          LedBlink(ORANGE,LED_4HZ);
          LedBlink(RED,LED_8HZ);
          LedOff(WHITE);
          LedOff(PURPLE);
          LedOff(BLUE);
          LedOff(CYAN);
          LedPWM(LCD_RED,LED_PWM_55);
          LedPWM(LCD_GREEN,LED_PWM_30);
          LedPWM(LCD_BLUE,LED_PWM_15);
          
          /*function of Lcd*/
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR+7,"STATE 2");
          
          /*Back to the UserApp1SM_Idle()*/
          UserApp1_StateMachine=UserApp1SM_Idle;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
