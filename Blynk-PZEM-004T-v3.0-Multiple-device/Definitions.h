/*
 * Definitions.h
 *
 *  Created on: Mar 17, 2020
 *      Author: E_CAD
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

	#include "MyModbusMaster.h"
	#include <SoftwareSerial.h>  //  ( NODEMCU ESP8266 )


	/***************************************************
	 *        NodeMCU Pin Assignment
	 **************************************************/
	#define RX1_PIN_NODEMCU     D5        // Nodemcu pin, used has RX pin
	#define TX1_PIN_NODEMCU     D6        // Nodemcu pin, used has TX pin


	/***************************************************
	 *        Server Settings
	 **************************************************/
	#define OTA_HOSTNAME "PZEM-004T"	// no dots, no spaces!
										// after device online in LAN you can check if its alive by ping to OTA_HOSTNAME.local


	// Build in LED
	#define LED_BUILTIN 2

	// Definition der Debuglevel
	#define DEBUG_ALWAYS 	0
	#define DEBUG_ERROR 	1
	#define DEBUG_WARNING 	2
	#define DEBUG_MIN_INFO 	3
	#define DEBUG_MED_INFO 	4
	#define DEBUG_MAX_INFO 	5
	// Wieviele Informationen sollenber die serielle Schnittstelle ausgegeben werden?
	#define DEBUG DEBUG_MED_INFO

	struct measure {
		float 		max;
		float 		min;
		float 		avg;
		uint32_t 	count;
	};


	// Single sensor measurements object
	class measurement {
	private:
		float 			sum; 			// summ of measurements in accumulator
		unsigned long 	first_ms;		// first measurement timestamp (ms)
		unsigned long 	Tmin_ms;		// Minimum measurement accumulation period (ms)
		unsigned long 	Tmax_ms;		// Maximum measurement accumulation period (ms)
		struct measure 	Meas_2_Store;	// Accumulated data to google sheets

	public:
		struct measure	Measurements;					// Measurement accumulation
		void NewMeas(float Measure, float treshold); 	// Store new measurement to accumulator
		void AddMeas(float Measure); 					// Add new measurement to accumulator (summarize)
		void Clear();									// Push measured values to archive
		void Stored();									// Data stored to google spreadsheets and buffer should be cleared
		String DebugAvg();								// Returns actual average value for debug
		String DebugRange();							// Returns string in MIN:MAX format
		String GetJson();
		measurement();									// constructor
		uint32_t GetCount_2_Store();
		bool Check_2_Store(unsigned min_sec, unsigned max_sec); // Check if measurement is ready to storage
		bool setCycles(unsigned int Tmin_sec, unsigned int Tmax_sec); //set parameters of maximum and minimum measurement cycle in seconds

	};


	// Electricity meter
	class Meter {
	private:
		uint32_t 		CRCerr;		// number of CRC errors
		MyModbusMaster  MBNode;
		double 			PREV_active_energy;

	public:
		float			CRCerrRate 	= 0.0;	// Rate of CRC errors (%)
		float 			Divisor		= 1.0;  // Measurement head gain compensation
		int				ID			= 0;

		measurement		VOLTAGE;
		measurement		CURRENT_USAGE;
		measurement		ACTIVE_POWER;
		measurement		ACTIVE_ENERGY;
		measurement		FREQUENCY;
		measurement		POWER_FACTOR;

		void   GetData();				// Get new measurements from device
		double GetLastEnergy();			// Get last measured Energy counter
		void   ResetEnergy();			// Reset energy counter
		void   Clear();					// Push measured values to archive
		void   Stored();				// Data stored to google spreadsheets and buffer should be cleared
		void   CRCError();				// Increase CRC error counter
		String DebugCRC();				// Returns CRC errors rate and measurements count
		String GetJson();
		void   begin(uint8_t pzemSlaveAddr, SoftwareSerial *pzemSerial, unsigned int Tmin_sec, unsigned int Tmax_sec);
		Meter();
		bool   Check_2_Store();			// Check if any data to be stored
	};


	String Float2String(const double value);
	String Float2String(const double value, uint8_t digits);
	String Float2String(const double value, uint8_t digits, uint8_t size);
	String check_display_value(double value, double undef, uint8_t len, uint8_t str_len);

	String Var2Json(const String& name, const String& value);
	String Var2Json(const String& name, const bool value);
	String Var2Json(const String& name, const int value);
	String Var2Json(const String& name, const double value);
	String ValueLocated2Json(const String& timestamp, const String& lat, const String& lng, const String& value);
	void debug_out(const String& text, const int level, const bool linebreak);

#endif /* DEFINITIONS_H_ */
