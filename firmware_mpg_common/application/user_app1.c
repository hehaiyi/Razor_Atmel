/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
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
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */
AntAssignChannelInfoType sAntSetupData;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_sMasterChannel;
static AntAssignChannelInfoType UserApp1_sSlaveChannel;

static void UserApp1SM_AntConfigureMaster();
static void UserApp1SM_AntConfigureSlave();
void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_);

static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
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
void UserApp1Initialize(void)
{
  /*Introduction*/
  u8 au8Title[] = "Hide and Seek";
  u8 au8Instructions[] = "B0 Begin Game";
  
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8Title); 
  LCDMessage(LINE2_START_ADDR, au8Instructions);
  
  LedOff(RED);
  LedOff(ORANGE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(CYAN);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(WHITE);
  LedOn(LCD_GREEN);
  LedOff(LCD_BLUE);
  LedOff(LCD_RED);

  /* Configure Slave ANT for this application */
  UserApp1_sSlaveChannel.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sSlaveChannel.AntChannelType      = CHANNEL_TYPE_SLAVE;
  UserApp1_sSlaveChannel.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sSlaveChannel.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

  UserApp1_sSlaveChannel.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sSlaveChannel.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sSlaveChannel.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sSlaveChannel.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sSlaveChannel.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sSlaveChannel.AntTxPower          = ANT_TX_POWER_USERAPP;
  UserApp1_sSlaveChannel.AntNetwork = ANT_NETWORK_DEFAULT;
  
    /* Configure Master ANT for this application */
  UserApp1_sMasterChannel.AntChannel          = ANT_CHANNEL_USERAPP;
  UserApp1_sMasterChannel.AntChannelType      = CHANNEL_TYPE_MASTER;
  UserApp1_sMasterChannel.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sMasterChannel.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

  UserApp1_sMasterChannel.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sMasterChannel.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sMasterChannel.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sMasterChannel.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sMasterChannel.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sMasterChannel.AntTxPower          = ANT_TX_POWER_USERAPP;
  UserApp1_sMasterChannel.AntNetwork = ANT_NETWORK_DEFAULT;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }

  UserApp1_u32Timeout = G_u32SystemTime1ms;
  /* If good initialization, set state to Idle */
  if(AntAssignChannel(&UserApp1_sSlaveChannel))
  {
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }


} /* end UserApp1Initialize() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void UserApp1SM_AntConfigureSlave(void)
{
  /* If good, set state to WaitChannelAssign */
  if( AntAssignChannel(&UserApp1_sSlaveChannel) )
  {
    /* Channel assignment is queued so start timer */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
   
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

static void UserApp1SM_AntConfigureMaster(void)
{
  /* If good, set state to WaitChannelAssign */
  if( AntAssignChannel(&UserApp1_sMasterChannel) )
  {
    /* Channel assignment is queued so start timer */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
   
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}


/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }
  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
      
} /* end UserApp1SM_WaitChannelAssign() */

static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_StateMachine = UserApp1SM_Error;
    
  }
    
} /* end UserApp1SM_WaitChannelOpen() */

static void UserApp1SM_WaitChannelClose(void)
{
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    UserApp1_StateMachine = UserApp1SM_Error;
  }
    
} /* end UserApp1SM_WaitChannelClose() */

static void UserApp1SM_Hider(void)
{
  static u8 au8TestMessage[]={0,0,0,0,0xA5,0,0,0};
  static u8 au8CountDown[]="x";
  static u16 u16CountDown=3001;
  static bool bCountEnd=FALSE;  
  
  /*the game don't begin untill Countdown end*/
  if(u16CountDown!=0&&!bCountEnd)
  {
    u16CountDown--;
    if(au8CountDown[0]%1000==0)
    {
      au8CountDown[0]=u16CountDown/1000+'0';
      LCDMessage(LINE1_START_ADDR,"CountDown:");
      LCDMessage(LINE1_START_ADDR+12,au8CountDown);    
    }

  }

  if(u16CountDown==0)
  {
    bCountEnd=TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE2_START_ADDR,"Status:Hide");
        
    if( AntReadAppMessageBuffer() )
    {
       /* New message from seeker, meaning found, go to found state */
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE2_START_ADDR,"Status:Founded");
      }
      else if(G_eAntApiCurrentMessageClass == ANT_TICK)
      {
       /* Update and queue the new message data */
        au8TestMessage[7]++;
        if(au8TestMessage[7] == 0)
        {
          au8TestMessage[6]++;
          if(au8TestMessage[6] == 0)
          {
            au8TestMessage[5]++;
          }
        }
        AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);
      }
    } /* end AntReadData() */
}
  
  UserApp1_StateMachine = UserApp1SM_Idle;
}

static void UserApp1SM_Seeker(void)
{
  LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, 
                                        CYAN, BLUE, PURPLE, WHITE};
  s8 as8dBmLevels[] = {DBM_LEVEL1, DBM_LEVEL2, DBM_LEVEL3, DBM_LEVEL4, 
                       DBM_LEVEL5, DBM_LEVEL6, DBM_LEVEL7, DBM_LEVEL8};  
  static u8 au8CountDown[]="x";
  static u8 au8FoundMessage[]= {1, 1, 1, 1, 1, 1, 1, 1};
  static s8 sSeekerRssi=-99;
  static u16 u16AfterFoundCounter=0;
  static u16 u16CountDown=3001;
  static bool bCountEnd=FALSE;
  static bool bShowStatus=TRUE;
  static bool bSendMessageToHider=FALSE;

  /*Show the CountDown*/
  if(u16CountDown!=0&&!bCountEnd)
  {
    u16CountDown--;
    if(au8CountDown[0]%1000==0)
    {
      au8CountDown[0]=u16CountDown/1000+'0';
      LCDMessage(LINE1_START_ADDR,"CountDown:");
      LCDMessage(LINE1_START_ADDR+12,au8CountDown);    
    }
  }
  
  if(u16CountDown==0)
  {
    if(bShowStatus)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Ready or not");
      LCDMessage(LINE2_START_ADDR,"Here I come!");
      bShowStatus=FALSE;
    }
    
    /*Get the messages from Hider*/
    if( AntReadAppMessageBuffer() )
    {
      /* New data message: check what it is */
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
        {
          if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
          {
            au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];
          }
        }
        
        /*Get the Rssi*/
        sSeekerRssi=G_sAntApiCurrentMessageExtData.s8RSSI;
        
        /* Loop through all of the levels to check which LEDs to turn on */
        for(u8 i = 0; i < NUM_DBM_LEVELS; i++)
        {
          if(sSeekerRssi > as8dBmLevels[i])
          {
            LedOn(aeLedDisplayLevels[i]);
          }
          else
          {
            LedOff(aeLedDisplayLevels[i]);
          }
        }
        
        if(sSeekerRssi==DBM_LEVEL8)
        {
          LCDCommand(LCD_CLEAR_CMD);          
          LCDMessage(LINE1_START_ADDR,"Found you!");     
          AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8FoundMessage);
          bSendMessageToHider=TRUE;
          u16CountDown=3001;
        }
      } /*end if(G_eAntApiCurrentMessageClass == ANT_DATA)*/    
    }/*end AntReadAppMessageBuffer()*/
    
    /*After found the hider*/
    if(bSendMessageToHider)
    {
      u16AfterFoundCounter++;
      LedBlink(WHITE,LED_4HZ);
      LedBlink(PURPLE,LED_4HZ);
      LedBlink(BLUE,LED_4HZ);
      LedBlink(CYAN,LED_4HZ);
      LedBlink(GREEN,LED_4HZ);
      LedBlink(YELLOW,LED_4HZ);
      LedBlink(ORANGE,LED_4HZ);
      LedBlink(RED,LED_4HZ);
    }
    
    /*10s countdown*/
    if(u16AfterFoundCounter==10000)
    {
      bSendMessageToHider=FALSE;
      u16AfterFoundCounter=0;
      LedOff(WHITE);
      LedOff(PURPLE);
      LedOff(BLUE);
      LedOff(CYAN);
      LedOff(GREEN);
      LedOff(YELLOW);
      LedOff(ORANGE);
      LedOff(RED);
      PWMAudioSetFrequency(BUZZER1,0);
      
      AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_u32Timeout=G_u32SystemTime1ms;
      UserApp1_StateMachine=UserApp1SM_WaitChannelClose;
    }
  }
}
  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{
  static bool bChooseTheHider=FALSE; 
  static bool bReseted=TRUE;
  static bool bGotNewData = FALSE;
  static bool bDisplay = FALSE;
  
  
  if(!bDisplay)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR,"Hide and Go Seek!");
    LCDMessage(LINE2_START_ADDR,"Press B0 to Start");
  }
  
  if(bReseted)
  {
    /* Check if got new data */
    if(AntReadAppMessageBuffer())
    {
      if(G_eAntApiCurrentMessageClass==ANT_DATA)
      {
        bGotNewData=FALSE;
        for(u8 i =0;i<ANT_APPLICATION_MESSAGE_BYTES;i++)
        {
          if(G_au8AntApiCurrentMessageBytes[i]!=au8LastAntData[i])
          {
            bGotNewData=TRUE;
            au8LastAntData[i]=G_au8AntApiCurrentMessageBytes[i];
          }
        }
      }
      /* When got new data, meaning pared, go to the seeker state */
      if(bGotNewData==TRUE)
      {
        bReseted=FALSE;
        UserApp1_StateMachine=UserApp1SM_Seeker;
      }
    }
    
    /* Look for BUTTON 0 to open channel */
    if(WasButtonPressed(BUTTON0))
    {
      /* Got the button, so complete one-time actions before next state */
      ButtonAcknowledge(BUTTON0);

      if(bChooseTheHider==FALSE)
      {
        AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
        AntUnassignChannelNumber(ANT_CHANNEL_USERAPP);
        AntAssignChannel(&UserApp1_sMasterChannel);
        UserApp1_u32Timeout=G_u32SystemTime1ms;
        bChooseTheHider=TRUE;
        UserApp1_StateMachine=UserApp1SM_AntConfigureMaster;
      }           
    }
    else
    {
      /*the sSlaveChannel unassign*/
      if(!AntAssignChannel(&UserApp1_sSlaveChannel))
      {
        AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
        AntUnassignChannelNumber(ANT_CHANNEL_USERAPP);
        AntAssignChannel(&UserApp1_sSlaveChannel);
        UserApp1_u32Timeout=G_u32SystemTime1ms;
        UserApp1_StateMachine=UserApp1SM_AntConfigureSlave;      
      }
    }
    
    /*Go to the Hider state*/
    if(bChooseTheHider==TRUE)
    {
      bDisplay=TRUE;
      bReseted=TRUE;
      UserApp1_StateMachine=UserApp1SM_Hider;
    }
  }   
} /* end UserApp1SM_Idle() */
     




/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
