/**********************************************************************************************************************
File: user_app1.c                                                                

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
  /*pin Initialize*/
  AT91C_BASE_PIOB->PIO_OER=PB_03_BLADE_AN0;
  AT91C_BASE_PIOA->PIO_OER=PA_11_BLADE_UPIMO;
  AT91C_BASE_PIOB->PIO_PER=PB_03_BLADE_AN0;
  AT91C_BASE_PIOA->PIO_PER=PA_11_BLADE_UPIMO;

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
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
  static u16 au16NotesLeft[]    = {F4, F4, A4, A4, D4, D4, F4, F4, A3S, A3S, D4, D4, C4, C4, E4, E4};
  static u16 au16DurationLeft[] = {EN, EN, EN, EN, EN, EN, EN, EN, EN,  EN,  EN, EN, EN, EN, EN, EN};
  static u8 u8LedIntervalTime=0;
  static u8 u8DurationCount=0;
  static u8 u8NoteCount=0;
  static bool bBlink=FALSE;
  static bool bBuzzer=FALSE;
  

  /*if press Button0*/
  if(AT91C_BASE_PIOA->PIO_PDSR & PA_17_BUTTON0)  
  {
    AT91C_BASE_PIOB->PIO_CODR=PB_20_LED_RED;
    AT91C_BASE_PIOB->PIO_CODR=PB_19_LED_GRN;
    AT91C_BASE_PIOB->PIO_CODR=PB_03_BLADE_AN0;
    AT91C_BASE_PIOA->PIO_CODR=PA_11_BLADE_UPIMO;
    
    bBlink=FALSE;
    bBuzzer=FALSE;
    
    PWMAudioOff(BUZZER1);
  }
  else
  {
    /*only the button pressed work*/
    u8DurationCount++;
    u8LedIntervalTime++;
    bBlink=TRUE;
    bBuzzer=TRUE;
  }
  
  
  /*
  if(AT91C_BASE_PIOA->PIO_PDSR & PA_15_BLADE_SCK)  
  {
    AT91C_BASE_PIOB->PIO_CODR=PB_20_LED_RED;
    AT91C_BASE_PIOB->PIO_CODR=PB_19_LED_GRN;
    AT91C_BASE_PIOB->PIO_CODR=PB_03_BLADE_AN0;
    AT91C_BASE_PIOA->PIO_CODR=PA_11_BLADE_UPIMO;
    bBlink=FALSE;
    bBuzzer=FALSE;
    PWMAudioOff(BUZZER1);
  }
  else
  {
    u8DurationCount++;
    u8LedIntervalTime++;
    bBlink=TRUE;
    bBuzzer=TRUE;
  }
  */
  
  /*set time and make led blink*/
  if(u8LedIntervalTime==100&&bBlink)
  {
    AT91C_BASE_PIOB->PIO_CODR=PB_03_BLADE_AN0;
    AT91C_BASE_PIOA->PIO_SODR=PA_11_BLADE_UPIMO;
    AT91C_BASE_PIOB->PIO_CODR=PB_20_LED_RED;
    AT91C_BASE_PIOB->PIO_SODR=PB_19_LED_GRN;
  }
  
  if(u8LedIntervalTime==200&&bBlink)
  {
    AT91C_BASE_PIOB->PIO_SODR=PB_03_BLADE_AN0;
    AT91C_BASE_PIOA->PIO_CODR=PA_11_BLADE_UPIMO;
    AT91C_BASE_PIOB->PIO_SODR=PB_20_LED_RED;
    AT91C_BASE_PIOB->PIO_CODR=PB_19_LED_GRN;
    u8LedIntervalTime=0;
  }
  
  /*set Buzzer*/
  if(u8DurationCount==au16DurationLeft[u8NoteCount]&&bBuzzer==TRUE)
  {
    u8DurationCount=0;
    u8NoteCount++;
    PWMAudioSetFrequency(BUZZER1,au16NotesLeft[u8NoteCount]);
    PWMAudioOn(BUZZER1);
  }
  
  /*implement music loop*/
  if(u8NoteCount==(sizeof(au16DurationLeft)/2-1))
  {
    u8NoteCount=0;
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
