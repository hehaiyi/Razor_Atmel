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
void UserApp1Initialize(void)
{
 
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
  u8 u8RealName[5]={'h','a','i','y','i'};                           /*My real Name*/
  u8 u8InputName[10];                                               /*Input Name*/
  static u8 u8AppearNameCount=0;                                   /*Frequency of my name appear*/
  static u8 u8NameLetterCorrectOrderAppearCount=0;                 /*Frequency of every letter appear */                              
  static u8 u8RiseMarkNumber=10;                                   /*Sign of increase the mark number*/                                  
  static u8 u8MarkNumber=3;                                        /*Original number of mark */
  static u8 u8PositionOfRealNameArrray=0;                          /*Position of the Array */
  
  if(G_u8DebugScanfCharCount==1)                                    /*Each letter Input*/
   {    
     DebugScanf(u8InputName);                                       /*Reset G_u8DebugScanfCharCount and Put the letter into the u8InputName[0]*/
     if(u8InputName[0]==u8RealName[u8PositionOfRealNameArrray])     /*Match the first letter */
     { 
        u8NameLetterCorrectOrderAppearCount++;                       /*Number of letter successful comparison increase*/
        u8PositionOfRealNameArrray++;                                /*Turn to next letter will be match*/              
      }
      else                                                           /*Faild match*/
      {     
        u8PositionOfRealNameArrray=0;                                 
        u8NameLetterCorrectOrderAppearCount=0;                      
        if(u8InputName[0]=u8RealName[u8PositionOfRealNameArrray])     /*avoid two top letter close togerther*/
         {
            u8PositionOfRealNameArrray++;
            u8NameLetterCorrectOrderAppearCount++;
          }
        else
         {
           u8PositionOfRealNameArrray=0;
           u8NameLetterCorrectOrderAppearCount=0;
         }
      }
    }
    
   if(u8NameLetterCorrectOrderAppearCount==5)                         /*Reach the matching number*/
     {  
       u8NameLetterCorrectOrderAppearCount=0;
       u8AppearNameCount++;                                          /*Number of Name appear ++*/
       u8PositionOfRealNameArrray=0;                                  
        
       if(u8AppearNameCount==u8RiseMarkNumber)                       /*Figures increase than the number of mark increase*/
         {
            u8MarkNumber++;
            u8RiseMarkNumber=10*u8RiseMarkNumber;
         }
        
        DebugLineFeed();                                                /*Show the number name appear*/
        for(u8 u8MarkCount=0;u8MarkCount<u8MarkNumber;u8MarkCount++)
        {
          DebugPrintf("*");
        }
        DebugLineFeed();
        DebugPrintf("*");
        DebugPrintNumber(u8AppearNameCount);
        DebugPrintf("*");
        DebugLineFeed();
        for(u8 u8MarkCount=0;u8MarkCount<u8MarkNumber;u8MarkCount++)
        {
          DebugPrintf("*");
        }
        DebugLineFeed();
     }
  
    
  
  
  
  
  
  
  
  
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
