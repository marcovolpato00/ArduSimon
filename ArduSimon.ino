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
const int maxLevel = 5;
const int speed = 500;

int buttonPin[4] = { YELLOW_BUTTON, GREEN_BUTTON, BLUE_BUTTON, RED_BUTTON };
int ledPin[4] = { YELLOW_LED, GREEN_LED, BLUE_LED, RED_LED };
String ledPinNames[4] = { "Yellow", "Green", "Blue", "Red" };

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

	generateSequence();
	lcd.print("ArduSimon");
	delay(1000);
}

void loop()
{
	if (level <= maxLevel)
	{
		lcdPrint("Livello " + String(level), 0);
		delay(1000);
		countdown();

		showSequence();
		lcdPrint("Inserisci la", 0);
		lcdPrint("sequenza", 1);
		getSequence();
	}
	else
	{
		finalFlash();
		lcdPrint("ArduSimon", 0);
	}
}

bool sequenceIsCorrect(int counter)		// check if the sequence is correct
{
	if (userSequence[counter] != sequence[counter])
		return false;
	return true;
}

void sequenceCorrect()		// runs when the sequence is correct
{
	lcdPrint("Sequenza", 0);
	lcdPrint("corretta!", 1);
	flashAll();
	level++;
	delay(1000);
}

void sequenceWrong()		// runs when the sequence is wrong
{
	lcdPrint("Sequenza non", 0);
	lcdPrint("corretta...", 1);
	level = startLevel;
	delay(1000);
}

void getSequence()
{
	bool buttonPressed;

	for (int i = 0; i < level; i++)
	{
		buttonPressed = false;

		while (!buttonPressed)
		{
			for (int j = 0; j < 4; j++)
			{
				if (digitalRead(buttonPin[j]) == LOW)
				{
					int led = ledPin[j];
					flash(led);
					userSequence[i] = led;

					buttonPressed = true;
					
					if (!sequenceIsCorrect(i))
					{
						sequenceWrong();
						return;				// return to main loop
					}
				}
			}
		}
	}

	sequenceCorrect();
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
	}
}

void generateSequence()
{
	randomSeed(analogRead(A0));

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
	delay(200);
	digitalWrite(led, LOW);
  delay(200);
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

void finalFlash()
{
	digitalWrite(YELLOW_LED, HIGH);
	delay(200);
	digitalWrite(YELLOW_LED, LOW);
	digitalWrite(GREEN_LED, HIGH);
	delay(200);
	digitalWrite(GREEN_LED, LOW);
	digitalWrite(BLUE_LED, HIGH);
	delay(200);
	digitalWrite(BLUE_LED, LOW);
	digitalWrite(RED_LED, HIGH);
	delay(200);
	digitalWrite(RED_LED, LOW);
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
