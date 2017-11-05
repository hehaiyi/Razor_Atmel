/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static u32 UserApp_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static u32 UserApp_u32TickMsgCount = 0;             /* Counts the number of ANT_TICK packets received */

static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static void UserAppSM_Hider(void);
static void UserAppSM_Seeker(void);
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
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  u8 au8Title[] = "Hide and Seek";
  u8 au8Instructions[] = "B0 Begin Game";
  
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8Title); 
  LCDMessage(LINE2_START_ADDR, au8Instructions); 
   
 /* Configure ANT for this application */
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_USERAPP;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_USERAPP;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;
  
  /* If good initialization, set state to Idle */
  if(1)
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    UserApp_StateMachine = UserAppSM_Error;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static bool bHide=FALSE;
  static bool bSeeker=FALSE;

  /* Look for BUTTON 0 to choose which is Hider */
  if(WasButtonPressed(BUTTON0))
  {
    AntCloseChannel();
    AntChannelConfig(ANT_MASTER);
    AntOpenChannel();

    bHide=TRUE;
  }
  /*the other device recieve the message from Hider and become Seeker*/
  else if(AntReadData())
  {  
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      AntCloseChannel();
      AntChannelConfig(ANT_SLAVE);
      AntOpenChannel();   

      bSeeker=TRUE;
    }
  }
  if(bHide)
  {
    UserApp_StateMachine = UserAppSM_Hider;
  }
  if(bSeeker)
  {
    UserApp_StateMachine = UserAppSM_Seeker;
  }
} /* end UserAppSM_Idle() */
     
static void UserAppSM_Hider(void)
{
  static u8 au8HiderMessage[]={0,0,0,0,0,0,0,0};
  static bool bHiderFound=FALSE;
  
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,"HIDE!");
  
  if(!bHiderFound)
  {
    if(AntReadData())
    {
      if(G_eAntApiCurrentMessageClass == ANT_TICK)
      {
        AntQueueBroadcastMessage(au8HiderMessage);
      }

      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        bHiderFound=TRUE;
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,"Found You");
        
        UserApp_StateMachine=UserAppSM_Idle;
      }
    }
  }
}

static void UserAppSM_Seeker(void)
{
  static u8 au8SeekerMessage[]={0,0,0,0,0,0,0,0};
  static u8 u8CountDown[]={10};
  static bool bCountEnd=FALSE;
  static bool bBeginRecieveMessageFromHider=FALSE;
  
  if(u8CountDown[0]>=0&&!bCountEnd)
  {
    u8CountDown[0]--;
    LCDMessage(LINE1_START_ADDR,"CountDown:");
    LCDMessage(LINE1_START_ADDR+12,u8CountDown);
  }
  
  if(u8CountDown[0]==0)
  {
    bCountEnd=TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Here I come!");
  }
  
  if(AntReadData());
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserAppSM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatus() == ANT_OPEN)
  {
#ifdef MPG1
    LedOn(GREEN);
#endif /* MPG1 */    

    
    UserApp_StateMachine = UserAppSM_ChannelOpen;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannel();

#ifdef MPG1
    LedOff(GREEN);
    LedOn(YELLOW);
#endif /* MPG1 */    

    
    UserApp_StateMachine = UserAppSM_Idle;
  }
    
} /* end UserAppSM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Channel is open, so monitor data */
static void UserAppSM_ChannelOpen(void)
{
  static u8 u8LastState = 0xff;
  static u8 au8TickMessage[] = "EVENT x\n\r";  /* "x" at index [6] will be replaced by the current code */
  static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  bool bGotNewData;

  /* Check for BUTTON0 to close channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue close channel and change LED to blinking green to indicate channel is closing */
    AntCloseChannel();
    u8LastState = 0xff;

#ifdef MPG1
    LedOff(YELLOW);
    LedOff(BLUE);
    LedBlink(GREEN, LED_2HZ);
#endif /* MPG1 */    

    
    /* Set timer and advance states */
    UserApp_u32Timeout = G_u32SystemTime1ms;
    UserApp_StateMachine = UserAppSM_WaitChannelClose;
  } /* end if(WasButtonPressed(BUTTON0)) */
  
  /* Always check for ANT messages */
  if( AntReadData() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      UserApp_u32DataMsgCount++;
      

    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      UserApp_u32TickMsgCount++;

    } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
    
  } /* end AntReadData() */
  
  /* A slave channel can close on its own, so explicitly check channel status */
  if(AntRadioStatus() != ANT_OPEN)
  {
#ifdef MPG1
    LedBlink(GREEN, LED_2HZ);
    LedOff(BLUE);
#endif /* MPG1 */

    u8LastState = 0xff;
    
    UserApp_u32Timeout = G_u32SystemTime1ms;
    UserApp_StateMachine = UserAppSM_WaitChannelClose;
  } /* if(AntRadioStatus() != ANT_OPEN) */
      
} /* end UserAppSM_ChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to close */
static void UserAppSM_WaitChannelClose(void)
{
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatus() == ANT_CLOSED)
  {
#ifdef MPG1
    LedOff(GREEN);
    LedOn(YELLOW);
#endif /* MPG1 */

    UserApp_StateMachine = UserAppSM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp_u32Timeout, TIMEOUT_VALUE) )
  {
#ifdef MPG1
    LedOff(GREEN);
    LedOff(YELLOW);
    LedBlink(RED, LED_4HZ);
#endif /* MPG1 */

    
    UserApp_StateMachine = UserAppSM_Error;
  }
    
} /* end UserAppSM_WaitChannelClose() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{

} /* end UserAppSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
