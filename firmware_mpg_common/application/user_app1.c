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
  u8 au8UserMenuAndListDisplay[]="LED Programming Interface\n\rPress 1 to program LED command sequence\n\rPress 2 to show current USER program";
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  DebugPrintf(au8UserMenuAndListDisplay);
  
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

/*function is the user input the correct code shown one by one
  ListSize is listnode count*/
static u8 Output(u8 ListSize)
{ /*output interface*/


  static u8 auUserTips[]="If you want to add new command,input 3\n\r";

  static u8 au8UserProgram[]="Current USER Program:";
  static u8 au8DisplayName[]="LED  ON TIME    OFF TIME";
  static u8 au8SymbolDisplay[]="-----------------------";
  static bool bEnterFlag=TRUE;
  static u8 u8Count=0;
  /*allow appear output interface*/
  if(bEnterFlag == TRUE)
  {
     DebugPrintf(au8UserProgram);
     DebugPrintf("\r\n");
     DebugPrintf(au8DisplayName);
     DebugPrintf("\r\n");
     DebugPrintf(au8SymbolDisplay);
     DebugPrintf("\r\n");

     bEnterFlag=FALSE;
  }
  /*list is null ,go back UserApp1SM_Idle*/
  if(ListSize == 0)
  {
    DebugPrintf("list is null\r\n");
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  { /*whether the list  over?*/
    if(LedDisplayPrintListLine(u8Count)!=FALSE)
    {
      u8Count++;
    }
    else
    {
      
      DebugPrintf("\r\n");

      // DebugPrintf(au8SymbolDisplay);
      //u8Count=0;

      DebugPrintf(au8SymbolDisplay);
      DebugPrintf("\r\n");
      DebugPrintf(auUserTips);
      u8Count=0;

      UserApp1_StateMachine = UserApp1SM_Idle;
       
    }

  }
 return 0;
}


static void UserApp1SM_Idle(void)
{
  static u8 auUserMenuandListDisplay[]="Enter command as LED-ONTIME-OFFTIME and press Enter\n\rTIME is in milliseconds,max 100 commands\n\rExample: R-100-200(Red on at 100ms and off at 200ms)\n\rPress Enter on blank line to end\n\r";
  static u8 auUserTips[]="If you want to add new command,input 3\n\r";
  static u8 auCheckProgramOrShow[1];                                           /*check input 1 or 2*/
  static u8 auCheckInput[1];                                                   /*check any factor*/
  static u8 u8CountMemberNumber=0;                                             /*check how many member had been input*/
  static bool bCheckProgramOrShow=TRUE;                                        /*decide input or show */
  static bool bBeginInputData=FALSE;                                           /*decide whether input */                                
  static LedNumberType eLED1;                                                  /*to store which Led*/     
  static u32 u32OnTime=0;                                                      /*to store On time*/ 
  static u32 u32OffTime=0;                                                     /*to store Off time*/
  static u8 u8NumberOfUserList=0;                                              /*to show how many instructions be store*/                                           
  LedCommandType aeUserList[200];
  static bool u8InputOnTimeCount=FALSE;
  static bool u8InputOffTimeCount=FALSE;
  static u8 u8CountInputOnTimeNumber=0; 
  static u8 u8CountInputOffTimeNumber=0;
  static u8 u8LedError[]="Don't show which Led";
  static u8 u8InputMuchMark[]="Too much '-'";
  static u8 u8InputExtraSpace[]="Extra space ' '";
  static u8 u8MarkCount=0;
  static u8 u8SpaceCount=0;
  static bool bIfInputMuchMark=FALSE;
  static bool bIfInputLed=FALSE;
  static bool bIfInputExtraSpace=FALSE;
  static u8 u8Countu8NumberOfUserList=0;


    
  if(G_u8DebugScanfCharCount==1)
  {
   
      if(!bBeginInputData)
      {
        DebugScanf(auCheckProgramOrShow);
      }
    

      if(auCheckProgramOrShow[0]=='1'&&bBeginInputData==FALSE)                  /*Enter mode 1 to input*/
      {   
        DebugPrintf("\n\r");
        bCheckProgramOrShow=FALSE;
        bBeginInputData=TRUE;
        DebugPrintf(auUserMenuandListDisplay);
      }
      

      if(auCheckProgramOrShow[0]=='2'&&bBeginInputData==FALSE)                  /*Enter mode 2 to show list*/
      { 
        DebugPrintf("\n\r");
        for(u8Countu8NumberOfUserList=u8NumberOfUserList;u8Countu8NumberOfUserList>1;u8Countu8NumberOfUserList--)
        {
          Output(u8NumberOfUserList);
        }
        DebugPrintf(auUserTips);
        bCheckProgramOrShow=TRUE;
        auCheckInput[0]=0;
      }
      

      if(auCheckProgramOrShow[0]=='3'&&bBeginInputData==FALSE)                  /*Input again*/

      {   
        DebugPrintf("\n\r");
        bCheckProgramOrShow=FALSE;
        bBeginInputData=TRUE;

        DebugPrintf(auUserMenuandListDisplay);

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
         if(auCheckInput[0]=='W'||auCheckInput[0]=='w')
         {
            eLED1=WHITE;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='P'||auCheckInput[0]=='p')
         {
            eLED1=PURPLE;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='B'||auCheckInput[0]=='b')
         {
            eLED1=BLUE;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='C'||auCheckInput[0]=='c')
         {
            eLED1=CYAN;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='G'||auCheckInput[0]=='g')
         {
            eLED1=GREEN;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='Y'||auCheckInput[0]=='y')
         {
            eLED1=YELLOW;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='O'||auCheckInput[0]=='o')
         {
            eLED1=ORANGE;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]=='R'||auCheckInput[0]=='r')
         {
            eLED1=RED;
            u8CountMemberNumber++;
            bIfInputLed=TRUE;
         }
         else if(auCheckInput[0]!=NULL&&auCheckInput[0]!='\r')
         {
            u8CountMemberNumber++;
         }
       }
           
         
           /*Check '-'*/    
           if(auCheckInput[0]=='-')
           {
              u8MarkCount++;
              if(u8MarkCount>2)
              {
                  bIfInputMuchMark=TRUE;
              }
           }
        
           
           /*Check Space*/
           if(auCheckInput[0]==32)
           {
              u8SpaceCount++;
              if(u8SpaceCount!=0)
              {
                 bIfInputExtraSpace=TRUE; 
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
              u8InputOnTimeCount=FALSE;
              u8CountMemberNumber++;
              u8CountInputOnTimeNumber=0;
           }
           
         
           /*input the of time*/
           if(u8InputOffTimeCount&&auCheckInput[0]!='-'&&auCheckInput[0]!='\r')
           {  
               u32OffTime=u32OffTime*10+(auCheckInput[0]-48);
               u8CountInputOffTimeNumber++;
           }
           else if(u8CountInputOffTimeNumber>0&&auCheckInput[0]=='\r')
           {
               u8InputOffTimeCount=FALSE;
               u8CountMemberNumber++;
               u8CountInputOffTimeNumber=0;
               
           }
           
          
          /*Store the data into arrat*/
          if(u8CountMemberNumber==3&&bIfInputLed&&!bIfInputMuchMark&&!bIfInputExtraSpace) 
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
              u8NumberOfUserList++;
          }


          /*Store the data into list and initial some varities*/
          if(u8NumberOfUserList!=0&&u8CountMemberNumber==0&&auCheckInput[0]=='\r')
          {
            bBeginInputData=FALSE;
            for(u8 x = 0; x <u8NumberOfUserList; x++)
           {
              LedDisplayAddCommand(USER_LIST, &aeUserList[x]);
           }
          }   
           if(auCheckInput[0]=='\r')
           {

              DebugPrintf("\n\r");               
              if(!bIfInputLed&&u8CountMemberNumber==3)
              {
                  DebugPrintf(u8LedError);
                  DebugPrintf("\n\r");
              }
            
              if(bIfInputMuchMark&&u8CountMemberNumber==3)
              {
                  DebugPrintf(u8InputMuchMark);
                  DebugPrintf("\n\r");
              }
              
              if(bIfInputExtraSpace&&u8CountMemberNumber==3)
              {
                  DebugPrintf(u8InputExtraSpace);
                  DebugPrintf("\n\r");
              }
              u8CountMemberNumber=0;
              bIfInputLed=FALSE;
              bIfInputMuchMark=FALSE;
              bIfInputExtraSpace=FALSE;
              u8MarkCount=0;            
              u8SpaceCount=0;
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
