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

u8 u8ButtonValue;
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
static u8 u8RealPassWord[]={1,1,1,2,2,2,3,3,3,3};       //The real Password

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
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(WHITE);
    LedOff(ORANGE);
    LedOn(RED);
    LedOff(CYAN);
    
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
  


    static u8 GetButton()                                                           //Get botton value
{  
  
    u8 u8ButtonValue=9;
    if(WasButtonPressed(BUTTON0))
    { ButtonAcknowledge(BUTTON0);

      u8ButtonValue=1;
     }
    
    
    if(WasButtonPressed(BUTTON1))
    { 
      ButtonAcknowledge(BUTTON1);
      u8ButtonValue=2;
     }
   
   
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      u8ButtonValue=3;
     }
    
    
   if(WasButtonPressed(BUTTON3))   
    {
      ButtonAcknowledge(BUTTON3); 
    }
    
    return u8ButtonValue;
    
}

static bool bModeofLock=FALSE;                                                 //Switching mode that Input or Revise               

static void UserApp1SM_Idle(void)
{    static u8 u8Counter=0;                                                    //The rank of Password
     static u8 u8InputPassWord[]={9,9,9,9,9,9,9,9,9,9,9};                      //The Input Password
     static u16 Ledcounter=0;                                                  //Flicker frequency of LED     
     static u8 u8LedblinkTime=0;                                               //Flicker time of LED
     u8 u8Index;                                                                //Number of arrays
     u8 u8isPassword=1;                                                         //Password right or wrong
     u8 TempButtonValue;                                                        //The button value
     
     
     
  if(!bModeofLock)   
 {   TempButtonValue=GetButton();                                               //Get the button value
       if(TempButtonValue!=9)
      {  
      u8LedblinkTime=6;                                                         //Set flicker time of LED
      u8InputPassWord[u8Counter]=TempButtonValue;
      u8Counter++;
      }
    
    Ledcounter++;
    if(u8InputPassWord[u8Counter-1]!=u8RealPassWord[u8Counter-1])               //If the single password is wrong
     {   
        if(u8LedblinkTime>0)
         {
    
            if(Ledcounter%200==0)
              { 
                Ledcounter=0;
                LedToggle(RED);
                u8LedblinkTime--;
              }
  
          }
            if(u8LedblinkTime==0)                                               //After prompt error
             {   
               u8Counter--;                                                     //Back to last single password until it right
              }
     }
   
   if(u8InputPassWord[u8Counter-1]==u8RealPassWord[u8Counter-1])                //If the single password is right
     {
        if(u8LedblinkTime>0)
         {
            if(Ledcounter%200==0)
              { 
                Ledcounter=0;
                LedToggle(GREEN);
                u8LedblinkTime--;
              }
  
          }
        
     }
     
    
    
         
     if(u8Counter==10&&IsButtonPressed(BUTTON3))                                //If 10 password all clear and press button3
   {     
     for(u8Index=0;u8Index<10;u8Index++)                                        //Check all the password
     {
       if(u8InputPassWord[u8Index]!=u8RealPassWord[u8Index])                    //If one of the password is wrong
       {
          u8isPassword=0;
          break;                                                                //Jump out loop
         
        }
      }
          if(u8isPassword)                                                      //Password is right
            {
             LedOn(GREEN);
           
             LedOff(RED);
            }
           else                                                                 //Password is wrong
            {
              LedOn(RED);
          
            }
     
        u8Counter=0;                                                            //The rank of intput password zero
     
    }
  
  
 }  
 else   
 {
    UserApp1_SetPassword();
 }
   
   
   if(IsButtonHeld(BUTTON3,1000))                                               //Press the button3 to enter revise status
    {       
       bModeofLock=!bModeofLock;                                       
    }
  
   
    
    
    
  
  

  
} /* end UserApp1SM_Idle() */


static void UserApp1_SetPassword(void)                                        //The function of altering Password
{ 
  
  static u8 u8NumberofRevise=0;
  static u8 u8Ledcounter1=0;                                                    //LED timer++
  u8 u8FixPassWord;                                                             //The number was replaced
  
         u8Ledcounter1++;                                               

         if(u8NumberofRevise<10)                                                      
          { 
            u8FixPassWord=GetButton();
            if(u8FixPassWord!=9)
            {
              u8RealPassWord[u8NumberofRevise]=u8FixPassWord;                   //Replace the Password
              u8NumberofRevise++;
            }
          }
        
         if(u8NumberofRevise<10)                                                //About Led
        {      
         if(u8Ledcounter1%200==0)
              { 
                u8Ledcounter1=0;
                LedToggle(RED);
                LedToggle(GREEN);
                 
              }
         } 
        else                                                                   //Finish revise Password
         {
           LedOff(GREEN); 
           LedOn(RED);
           u8NumberofRevise=0;
           bModeofLock=!bModeofLock;                                            //Change the status to Input
         }
       
   
     
}




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
