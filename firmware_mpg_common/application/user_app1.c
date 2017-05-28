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
  static u8 auCheckProgramOrShow[1];                                           /*check input 1 or 2*/
  static u8 auCheckInput[1];                                                   /*check any factor*/
  static u8 u8CountMemberNumber=0;                                             /*check how many member had been input*/
  static bool bCheckProgramOrShow=TRUE;                                        /*decide input or show */
  static bool bBeginInputData=FALSE;                                           /*decide whether input */                                
  static LedNumberType eLED1;                                                /*to store which Led*/     
  static u32 u32OnTime=0;                                                      /*to store On time*/ 
  static u32 u32OffTime=0;                                                     /*to store Off time*/
  static u8 u8NumberOfUserList=0;                                                                                          
  LedCommandType aeUserList[200];
  static bool u8InputOnTimeCount=FALSE;
  static bool u8InputOffTimeCount=FALSE;
  static u8 u8CountInputOnTimeNumber=0; 
  static u8 u8CountInputOffTimeNumber=0;

    
  if(G_u8DebugScanfCharCount==1)
  {
   
      if(!bBeginInputData)
        {
          DebugScanf(auCheckProgramOrShow);
        }
    
      if(auCheckProgramOrShow[0]=='1'&&bBeginInputData==FALSE)
        {   
          DebugPrintf("\n\r");
          bCheckProgramOrShow=!bCheckProgramOrShow;
          bBeginInputData=!bBeginInputData;
        }
      
      if(auCheckProgramOrShow[0]=='3'&&bBeginInputData==TRUE)
        {   
          DebugPrintf("\n\r");
          bCheckProgramOrShow=!bCheckProgramOrShow;
          bBeginInputData=!bBeginInputData;
        }
    
    
    
       if(u8CountMemberNumber==1&&!u8InputOnTimeCount)
         {
            u8InputOnTimeCount=TRUE;
            u32OnTime=0;
            u8CountInputOnTimeNumber=0;
         }
    
     if(u8CountMemberNumber==2&&!u8InputOffTimeCount)
        {
           u8InputOffTimeCount=TRUE;
           u32OffTime=0;
           u8CountInputOffTimeNumber=0;
        }
        DebugScanf(auCheckInput);
    
 /*input the first menber*/
     if(bCheckProgramOrShow==FALSE)
       {   
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
            }
             
           /*input the on time*/
           if(u8InputOnTimeCount&&auCheckInput[0]!='-')
             {
                u32OnTime=u32OnTime*10+(auCheckInput[0]-48);
                u8CountInputOnTimeNumber++;
             }
           else if(u8CountInputOnTimeNumber>0&&auCheckInput[0]=='-')
             {
                u8InputOnTimeCount=!u8InputOnTimeCount;
                u8CountMemberNumber++;
             }
            /*input the of time*/
           if(u8InputOffTimeCount&&auCheckInput[0]!='-'&&auCheckInput[0]!='\r')
             {  
                 u32OffTime=u32OffTime*10+(auCheckInput[0]-48);
                 u8CountInputOffTimeNumber++;
             }
           else if(u8CountInputOffTimeNumber>0&&auCheckInput[0]=='\r')
             {
                 u8InputOffTimeCount=!u8InputOffTimeCount;
                 u8CountMemberNumber++;
             }
           
           if(auCheckInput[0]=='\r') 
            {
               
                DebugPrintf("\n\r");
            }
          if(u8CountMemberNumber==3) 
            {   
             
                aeUserList[u8NumberOfUserList].eLED=eLED1;
                aeUserList[u8NumberOfUserList].u32Time=u32OnTime;
                aeUserList[u8NumberOfUserList].bOn=TRUE;
                aeUserList[u8NumberOfUserList].eCurrentRate=LED_PWM_0;
                u8NumberOfUserList++;
                aeUserList[u8NumberOfUserList].eLED=eLED1;
                aeUserList[u8NumberOfUserList].u32Time=u32OffTime;
                aeUserList[u8NumberOfUserList].bOn=FALSE;
                aeUserList[u8NumberOfUserList].eCurrentRate=LED_PWM_100;
                u8CountMemberNumber=0;
                u8NumberOfUserList++;
            }
           
         
          if(u8NumberOfUserList>0&&u8CountMemberNumber==0&&auCheckInput[0]=='\r')
            {
             
              for(u8 x = 0; x <u8NumberOfUserList; x++)
             {
                LedDisplayAddCommand(USER_LIST, &aeUserList[x]);
             }
            }   
       } 
 
  
  
  
  
  
  
  
  }
  
  
  

  
  
  
 
  
  
  
  
  
  
/*
 
  LedCommandType aeUserList[]=
  {
    {RED,0,TRUE,LED_PWM_0},
    
    {RED,500,FALSE,LED_PWM_100},
  
  
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
