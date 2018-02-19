/*
	   _          _      ___ _
	  /_\  _ _ __| |_  _/ __(_)_ __  ___ _ _
	 / _ \| '_/ _` | || \__ \ | '  \/ _ \ ' \
	/_/ \_\_| \__,_|\_,_|___/_|_|_|_\___/_||_|

	Marco Volpato 2018

*/

#include <LiquidCrystal.h>

#define YELLOW_LED	2
#define GREEN_LED	3
#define BLUE_LED	4
#define RED_LED		5

#define	YELLOW_BUTTON	A0
#define GREEN_BUTTON	A1
#define BLUE_BUTTON		A2
#define	RED_BUTTON		A3

// display pins
#define RS 11
#define EN 10
#define D4 9
#define D5 8
#define D6 7
#define D7 6

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const int startLevel = 1;
const int maxLevel = 20;
const int speed = 500;

int userSequence[maxLevel];
int sequence[maxLevel];
int level = startLevel;

void setup()
{
	// leds
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(BLUE_LED, OUTPUT);
	pinMode(RED_LED, OUTPUT);

	// buttons
	pinMode(YELLOW_BUTTON, INPUT_PULLUP);
	pinMode(GREEN_BUTTON, INPUT_PULLUP);
	pinMode(BLUE_BUTTON, INPUT_PULLUP);
	pinMode(RED_BUTTON, INPUT_PULLUP);

	lcd.begin(16, 2);
	Serial.begin(9600);

	lcd.print("ArduSimon");
	delay(1000);
	countdown();
}

void loop()
{
	if (level <= maxLevel)
	{
		generateSequence();
		showSequence();
		//Serial.println("Inserisci la sequenza");
		lcdPrint("Inserisci la", 0);
		lcdPrint("sequenza", 1);
		getSequence();

		if (sequenceIsCorrect())
		{
			//Serial.println("Sequenza corretta!");
			lcdPrint("Sequenza", 0);
			lcdPrint("corretta!", 1);
			flashAll();
			level++;
			lcdPrint("Livello " + String(level), 0);
			delay(1000);
		}
		else
		{
			//Serial.println("Sequenza non corretta! Riprova");
			lcdPrint("Sequenza non", 0);
			lcdPrint("corretta...", 1);
			delay(1000);
			level = startLevel;
			lcdPrint("Livello " + String(level), 0);
			delay(1000);
		}
		
		countdown();
	}
}

void lcdPrint(String message, int row)
{

	if (row == 1)
	{
		lcd.setCursor(0, 1);
	}
	else
	{
		lcd.clear();
		lcd.home();
	}

	lcd.print(message);
}

void countdown()
{
	lcdPrint("3", 0);
	delay(1000);
	lcdPrint("2", 0);
	delay(1000);
	lcdPrint("1", 0);
	delay(1000);
	lcd.clear();
}

void buttonPressed(int led, int counter)	// when a button is pressed
{
	flash(led);
	Serial.println(ledToString(led));
	userSequence[counter] = led;
}

void getSequence()
{
	for (int i = 0; i < level; i++)
	{
		while (true)
		{
			if (digitalRead(YELLOW_BUTTON) == LOW)
			{
				buttonPressed(YELLOW_LED, i);
				break;
			}
			else if (digitalRead(GREEN_BUTTON) == LOW)
			{
				buttonPressed(GREEN_LED, i);
				break;
			}
			else if (digitalRead(BLUE_BUTTON) == LOW)
			{
				buttonPressed(BLUE_LED, i);
				break;
			}
			else if (digitalRead(RED_BUTTON) == LOW)
			{
				buttonPressed(RED_LED, i);
				break;
			}
		}
	}
}

bool sequenceIsCorrect()
{
	for (int i = 0; i < level; i++)
	{
		if (userSequence[i] != sequence[i])
			return false;
	}
	return true;
}

void showSequence()
{
	turnOffAll();

	for (int i = 0; i < level; i++)
	{
		digitalWrite(sequence[i], HIGH);
		delay(speed);
		digitalWrite(sequence[i], LOW);
		delay(speed);
		Serial.println(ledToString(sequence[i]));
	}
}

String ledToString(int led)
{
	switch (led)
	{
		case 2:
			return "Giallo";
		case 3:
			return "Verde";
		case 4:
			return "Blu";
		case 5:
			return "Rosso";
		default:
			break;
	}
}

void generateSequence()
{
	randomSeed(millis());

	for (int i = 0; i < maxLevel; i++)
	{
		sequence[i] = random(2, 6);
	}
}

void turnOffAll()
{
	digitalWrite(YELLOW_LED, LOW);
	digitalWrite(GREEN_LED, LOW);
	digitalWrite(BLUE_LED, LOW);
	digitalWrite(RED_LED, LOW);
}

void flash(int led)
{
	digitalWrite(led, HIGH);
	delay(500);
	digitalWrite(led, LOW);
	delay(500);
}

void flashAll()
{
	for (int i = 1; i <= 4; i++)
	{
		digitalWrite(YELLOW_LED, HIGH);
		digitalWrite(GREEN_LED, HIGH);
		digitalWrite(BLUE_LED, HIGH);
		digitalWrite(RED_LED, HIGH);
		delay(200);
		turnOffAll();
		delay(200);
	}
}
