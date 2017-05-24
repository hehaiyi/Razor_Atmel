/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

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


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
void UserApp1Initialize(void)
{
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
  
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{ 
  static u8 auCheckProgramOrShow[1];
  static u8 auCheckInput[1];
  static u8 u8CountMemberNumber=0;
  static bool bCheckProgramOrShow=TRUE;
  static bool bBeginInputData=FALSE;
  static LedNumberType eLED1;       /* LED to operate on */
  static u32 u32Time1;              /* Time of action */
  static bool bOn1;                 /* TRUE if this is an ON event */
  static u8 i=1;
  LedCommandType aeUserList[200];
  static bool u8InputTimeCount=TRUE;
    
  if(!bBeginInputData)
    {
        DebugScanf(auCheckProgramOrShow);
   
    }
    if(auCheckProgramOrShow[0]=='1'&&bBeginInputData==FALSE)
    {   
        bCheckProgramOrShow=!bCheckProgramOrShow;
        bBeginInputData=!bBeginInputData;
    }
      
    if(bCheckProgramOrShow==FALSE)
      {   
           DebugScanf(auCheckInput);
         
           if(u8CountMemberNumber==0&&auCheckInput[0]!='-')
           { 
             if(auCheckInput[0]=='W')
             {
                eLED1=WHITE;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='P')
             {
                eLED1=PURPLE;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='B')
             {
                eLED1=BLUE;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='C')
             {
                eLED1=CYAN;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='G')
             {
                eLED1=GREEN;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='Y')
             {
                eLED1=YELLOW;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='O')
             {
                eLED1=ORANGE;
                u8CountMemberNumber++;
             }
             else if(auCheckInput[0]=='R')
             {
  
                eLED1=RED;
                u8CountMemberNumber++;
             }
   
             
             
             
               if(u8InputTimeCount&&u8CountMemberNumber==1&&auCheckInput[0]!='-')
                {
                    u32Time1=u32Time1*10+auCheckInput[0];
                }
               else if(u8CountMemberNumber==1&&auCheckInput[0]=='-')
               {
                    u8InputTimeCount=!u8InputTimeCount;
               }
                 
           
           
           }
            
             
            if(u8CountMemberNumber==3) 
            {   
             
                aeUserList[i].eLED=eLED1;
                aeUserList[i].u32Time=u32Time1;
                aeUserList[i].bOn=TRUE;
                aeUserList[i].eCurrentRate=LED_PWM_0;
                  i++;
                u8CountMemberNumber=0;
            }
            if(u8CountMemberNumber==0&&auCheckProgramOrShow[0]=='\r')
            {
              for(u8 i = 0; i < (sizeof(aeUserList) / sizeof(LedCommandType)); i++)
             {
                LedDisplayAddCommand(USER_LIST, &aeUserList[i]);
             }
          }
        }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
/* 
  LedCommandType aeUserList[]=
  {
    {RED,0,TRUE,LED_PWM_0},
    {YELLOW,500,TRUE,LED_PWM_0},
    {RED,500,FALSE,LED_PWM_100},
    {YELLOW,1000,FALSE,LED_PWM_100}
  
  };
   for(u8 i = 0; i < (sizeof(aeUserList) / sizeof(LedCommandType)); i++)
  {
    LedDisplayAddCommand(USER_LIST, &aeUserList[i]);
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
