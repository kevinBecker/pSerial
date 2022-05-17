/************************************************************/
/*    NAME: Mohamed Saad IBN SEDDIK, Adapted Kevin Becker   */
/*    ORGN: ENSTA Bretagne                                  */
/*    FILE: Serial.cpp                                      */
/*    DATE: 2022-5-17                                       */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Serial.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Serial::Serial()
{
  m_iterations = 0;
  m_timewarp   = 1;
}

//---------------------------------------------------------
// Destructor

Serial::~Serial()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Serial::OnNewMail(MOOSMSG_LIST &NewMail)
{
   return UpdateMOOSVariables(NewMail);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Serial::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Serial::Iterate()
{
  m_iterations++; 

  if(GetData())
    PublishData();

  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Serial::OnStartUp()
{
  CMOOSInstrument::OnStartUp();

 
  if (!m_Geodesy.Initialise(dLatOrigin,dLongOrigin))
  {
    MOOSTrace("Geodesy initialisation failed!!!");
    return false;
  }
  
  int max_retries = 5;
  double dSerialPeriod = 1.0;

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "Serial_PERIOD") {
        dSerialPeriod = atof(value.c_str());
      }
      else if(param == "MAX_RETRIES") {
        max_retries = atoi(value.c_str());
      }
    }
  }
  
  m_timewarp = GetMOOSTimeWarp();

  AddMOOSVariable(GetAppName()+"_IN", "", GetAppName()+"_IN", dSerialPeriod);
  AddMOOSVariable(GetAppName()+"_OUT", "", GetAppName()+"_OUT", dSerialPeriod);


  RegisterMOOSVariables();
  RegisterVariables();


  if (!SetupPort()) 
  {
    return false;
  }
  if (!(InitialiseSensorN(max_retries,"Serial")))
  {
    return false;
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Serial::RegisterVariable()
{
  // m_Comms.Register("FOOBAR", 0);
}

bool Serial::InitialiseSerial()
{
  MOOSPause(1000);
  if (m_Port.Flush()==-1)
  {
    return false;
  } else return true;
}

bool Serial::GetData()
{
  string sMessage;
  double dTime;

  if(m_Port.IsStreaming())
  {
    if (!m_Port.GetLatest(sMessage,dTime))
    {
      return false;
    }
  } else {
    if (!m_Port.GetTelegram(sMessage,0.5))
    {
      return false;
    }
  }

  return sMessage;
}

bool Serial::PublishData()
{
  return PublishFreshMOOSVariable();
}
