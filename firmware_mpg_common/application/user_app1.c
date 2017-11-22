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
static u32 UserApp1_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0;             /* Counts the number of ANT_TICK packets received */

static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static AntAssignChannelInfoType UserApp1_sMasterChannel;
static AntAssignChannelInfoType UserApp1_sSlaveChannel;

static void UserApp1SM_AntConfigureMaster();
static void UserApp1SM_AntConfigureSlave();
void AntGetdBmAscii(s8 s8RssiValue_, u8* pu8Result_);

static u8 UserApp1_au8Data[9]="0\0\0\0\0\0\0\0";
<<<<<<< HEAD
static bool bHiderAndSeeker=FALSE;
static u8 UserApp1_au8LcdInformationMessage[] = "M:-xx dBm S:-xx dbm";
static bool bFound=FALSE;
=======
static bool bHider=FALSE;
static bool bSeeker=FALSE;
static u8 UserApp1_au8LcdInformationMessage[] = "M:-xx dBm  S:-xx dBm";
>>>>>>> parent of 7b79244... 11.15 18：13
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
  UserApp1_sSlaveChannel.AntChannel          = ANT_CHANNEL_1;
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
  UserApp1_sSlaveChannel.AntChannel          = ANT_CHANNEL_0;
  UserApp1_sSlaveChannel.AntChannelType      = CHANNEL_TYPE_MASTER;
  UserApp1_sSlaveChannel.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  UserApp1_sSlaveChannel.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;

  UserApp1_sSlaveChannel.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  UserApp1_sSlaveChannel.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  UserApp1_sSlaveChannel.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  UserApp1_sSlaveChannel.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  UserApp1_sSlaveChannel.AntFrequency        = ANT_FREQUENCY_USERAPP;
  UserApp1_sSlaveChannel.AntTxPower          = ANT_TX_POWER_USERAPP;
  UserApp1_sSlaveChannel.AntNetwork = ANT_NETWORK_DEFAULT;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }

  AntAssignChannel(&UserApp1_sMasterChannel);
  UserApp1_u32Timeout = G_u32SystemTime1ms;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_AntConfigureMaster;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
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
  LedNumberType aeLedDisplayLevels[] = {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, PURPLE, WHITE};
  s8 as8dBmLevels[] = {DBM_LEVEL1, DBM_LEVEL2, DBM_LEVEL3, DBM_LEVEL4, 
                       DBM_LEVEL5, DBM_LEVEL6, DBM_LEVEL7, DBM_LEVEL8};
  u8 u8EventCode;
    
  static u8 u32MasterMessageCounter = 0;
  static s8 s8RssiChannel0 = -99;
  static s8 s8RssiChannel1 = -99;
  static s8 s8StrongestRssi = -99;

  /* Monitor ANT messages: looking for any incoming messages
  that indicates a matching device has been located. */
  if( AntReadAppMessageBuffer() )
  {
    /* Check the message class to determine how to process the message */
    if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      UserApp1_u32TickMsgCount++;
      /* Get the EVENT code from the ANT_TICK message */ 
      u8EventCode = G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX];
      
      /* Slave devices get different event codes than masters, so handle seperately */
      if(G_sAntApiCurrentMessageExtData.u8Channel == 0)
      {
        switch (u8EventCode)
        {
          case EVENT_TX:
          {
            /* Keep track of message and update LCD if too many messages have been sent
            without any being received.  The counter is cleared whenever the Master channel
            receives a message from the Slave it is trying to talk to. */
            u32MasterMessageCounter++;
            if(u32MasterMessageCounter >= 8)
            {
              s8RssiChannel0 = DBM_LEVEL1;
              LedOff(LCD_RED);
              UserApp1_au8LcdInformationMessage[INDEX_MASTER_DBM + 1] = 'x';
              UserApp1_au8LcdInformationMessage[INDEX_MASTER_DBM + 2] = 'x';
            }
            break;
          }
          default:
          {
            DebugPrintf("Master unhandled event\n\n\r");
            break;
          }
        } /* end switch u8EventCode */
      } /* end if(G_sAntApiCurrentMessageExtData.u8Channel == 0) */

      if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
      {
        /* Check the Event code and respond */
        switch (u8EventCode)
        {
          case EVENT_RX_FAIL_GO_TO_SEARCH:
          {
            s8RssiChannel1 = DBM_LEVEL1;
            LedOff(LCD_BLUE);
            UserApp1_au8LcdInformationMessage[INDEX_SLAVE_DBM + 1] = 'x';
            UserApp1_au8LcdInformationMessage[INDEX_SLAVE_DBM + 2] = 'x';
            break;
          }
          
          default:
          {
            DebugPrintf("Slave unhandled event\n\n\r");
            break;
          }
        } /* end switch u8EventCode */
      } /* end if(G_sAntApiCurrentMessageExtData.u8Channel == 1) */
    } /* end if(G_eAntApiCurrentMessageClass == ANT_TICK) */

    
    /* Check for DATA messages */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      UserApp1_u32DataMsgCount++;
      /* Check the channel number and update LED */
      if(G_sAntApiCurrentMessageExtData.u8Channel == 0)
      {
        /* Reset the message counter */
        u32MasterMessageCounter = 0;
        
        /* Channel 0 is red (but don't touch blue or green) */
        LedOn(LCD_RED);
        
        /* Record RSSI level and update LCD message */
        s8RssiChannel0 = G_sAntApiCurrentMessageExtData.s8RSSI;
        AntGetdBmAscii(s8RssiChannel0, &UserApp1_au8LcdInformationMessage[INDEX_MASTER_DBM]);
      }
      
      if(G_sAntApiCurrentMessageExtData.u8Channel == 1)
      {
        /* When the slave receives a message, queue a response message */
        AntQueueBroadcastMessage(ANT_CHANNEL_1, UserApp1_au8Data);

        /* Channel 1 is Blue (but don't touch red or green) */
        LedOn(LCD_BLUE);

        /* Record RSSI level and update LCD message */
        s8RssiChannel1 = G_sAntApiCurrentMessageExtData.s8RSSI;
        AntGetdBmAscii(s8RssiChannel1, &UserApp1_au8LcdInformationMessage[INDEX_SLAVE_DBM]);
      }
                            
      
    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */

    /* Make sure LCD has the current message - this should happen infrequently
    enough to no cause problems, but if that's untrue this needs to be throttled back */
    LCDMessage(LINE1_START_ADDR, UserApp1_au8LcdInformationMessage);
    
    /* Update the strongest signal being received */
    s8StrongestRssi = s8RssiChannel0;
    if(s8RssiChannel1 > s8RssiChannel0)
    {
      s8StrongestRssi = s8RssiChannel1;
    }

    /* Loop through all of the levels to check which LEDs to turn on */
    for(u8 i = 0; i < NUM_DBM_LEVELS; i++)
    {
      if(s8StrongestRssi > as8dBmLevels[i])
      {
        LedOn(aeLedDisplayLevels[i]);
      }
      else
      {
        LedOff(aeLedDisplayLevels[i]);
      }
    }
    
    if(s8StrongestRssi==DBM_LEVEL1)
    {
      bFound=TRUE;
    }
  }
} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void UserApp1SM_AntConfigureMaster(void)
{
    /* Wait for the ANT master channel to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)
  {
    DebugPrintf("Master channel configured\n\n\r");
    
    /* Queue configuration of Slave channel */
    AntAssignChannel(&UserApp1_sSlaveChannel);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 2000) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Master config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}

static void UserApp1SM_AntConfigureSlave(void)
{
    /* Wait for the ANT slave channel to be configured */
  if(AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CONFIGURED)
  {
    DebugPrintf("Slave channel configured\n\n\r");
    
    /* Update the broadcast message data to send the user's name the go to Idle */
    AntQueueBroadcastMessage(ANT_CHANNEL_0, UserApp1_au8Data);
    AntQueueBroadcastMessage(ANT_CHANNEL_1, UserApp1_au8Data);

    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout,2000) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Slave config failed");
    UserApp1_StateMachine = UserApp1SM_Error;    
  }
}

/*
static void UserApp1SM_HiderAndSeeker(void)
{
  static u8 au8HiderMessage[]={0,0,0,0,0,0,0,0};
  static bool bHiderFound=FALSE;
  
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR,"HIDE!");
  
  if(!bHiderFound)
  {
    UserApp1RunActiveState();
    AntQueueBroadcastMessage(ANT_CHANNEL_0,au8HiderMessage);

<<<<<<< HEAD
    if(bFound==TRUE)
    {
       bHiderFound=TRUE;
       LCDCommand(LCD_CLEAR_CMD);
       LCDMessage(LINE1_START_ADDR,"I was Found");
=======
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        bHiderFound=TRUE;
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR,"Found You");
        
        UserApp1_StateMachine=UserApp1SM_Idle;
      }
>>>>>>> parent of 7b79244... 11.15 18：13
    }
  }
  
  UserApp1_StateMachine=UserApp1SM_Idle;
}
*/

static void UserApp1SM_HiderAndSeeker(void)
{
  static u8 au8SeekerMessage[]={1,1,1,1,1,1,1,1};
  static u8 u8CountDown[]={10};
  static s8 sSeekerRssi=0;
  static bool bCountEnd=FALSE;
  static bool bBeginRecieveMessageFromHider;
  
  /*the game don't begin untill Countdown end*/
  if(u8CountDown[0]>=1&&!bCountEnd)
  {
    u8CountDown[0]--;
    LCDMessage(LINE1_START_ADDR,"CountDown:");
    LCDMessage(LINE1_START_ADDR+12,u8CountDown);
  }
  
  if(u8CountDown[0]==0)
  {
    bCountEnd=TRUE;
    bBeginRecieveMessageFromHider=TRUE;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE2_START_ADDR,"Hider:Coming!");
    LCDMessage(LINE2_START_ADDR,"Seeker:Here I come!");
     
    if(bBeginRecieveMessageFromHider)
    { 
      UserApp1RunActiveState();
    }  
    
    if(bFound==TRUE)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Seeker:Found you!");
      LCDMessage(LINE2_START_ADDR,"Hider:You found me!");
    }
  }
  
  UserApp1_StateMachine = UserApp1SM_Idle;
}



  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{
  static bool bChooseTheHider=FALSE; 
  static bool bChooseTheSeeker=FALSE;
  static u16 u16Count=0;
  
  u16Count++;

  /* Look for BUTTON 0 to open channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);

    if(bChooseTheHider==FALSE)
    {
      AntUnassignChannelNumber(ANT_CHANNEL_0);
      AntAssignChannel(&UserApp1_sMasterChannel);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      //AntOpenChannelNumber(ANT_CHANNEL_0);
      //AntOpenChannelNumber(ANT_CHANNEL_1);
      bChooseTheHider=TRUE;
      UserApp1_StateMachine = UserApp1SM_AntConfigureMaster;
    }   
  }
  
  if(bChooseTheSeeker==FALSE)
  {
    bChooseTheSeeker=TRUE;
    AntAssignChannel(&UserApp1_sSlaveChannel);
    UserApp1_StateMachine = UserApp1SM_AntConfigureSlave;
    AntOpenChannelNumber(ANT_CHANNEL_0);
    AntOpenChannelNumber(ANT_CHANNEL_1);

  }

  if(bHiderAndSeeker)
  {
    UserApp1_StateMachine = UserApp1SM_HiderAndSeeker;
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
