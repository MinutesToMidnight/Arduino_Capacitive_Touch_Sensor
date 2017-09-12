/*
Arduino_Capacitive_Touch_Sensor
   __    __     __     __   __     __  __     ______   ______     ______
  /\ "-./  \   /\ \   /\ "-.\ \   /\ \/\ \   /\__  _\ /\  ___\   /\  ___\
  \ \ \-./\ \  \ \ \  \ \ \-.  \  \ \ \_\ \  \/_/\ \/ \ \  __\   \ \___  \
   \ \_\ \ \_\  \ \_\  \ \_\\"\_\  \ \_____\    \ \_\  \ \_____\  \/\_____\
    \/_/  \/_/   \/_/   \/_/ \/_/   \/_____/     \/_/   \/_____/   \/_____/
                               ______   ______
                              /\__  _\ /\  __ \
                              \/_/\ \/ \ \ \/\ \
                                 \ \_\  \ \_____\
                                  \/_/   \/_____/
   __    __     __     _____     __   __     __     ______     __  __     ______
  /\ "-./  \   /\ \   /\  __-.  /\ "-.\ \   /\ \   /\  ___\   /\ \_\ \   /\__  _\
  \ \ \-./\ \  \ \ \  \ \ \/\ \ \ \ \-.  \  \ \ \  \ \ \__ \  \ \  __ \  \/_/\ \/
   \ \_\ \ \_\  \ \_\  \ \____-  \ \_\\"\_\  \ \_\  \ \_____\  \ \_\ \_\    \ \_\
    \/_/  \/_/   \/_/   \/____/   \/_/ \/_/   \/_/   \/_____/   \/_/\/_/     \/_/

  MinutesToMidnight
  Now with more fun in every byte!

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define speaker_pin 10
#define Excitor_pin 3
#define Sense_pin 7
#define LED_pin 13
#define time_out 32
#define touch_threshold 20
#define sensor_averaging 50

unsigned int zero_cal = 0;

void setup()
{
        // put your setup code here, to run once:
        pinMode(speaker_pin, OUTPUT);
        pinMode(Excitor_pin, OUTPUT);
        pinMode(LED_pin, OUTPUT);
        pinMode(Sense_pin, INPUT);
        Serial.begin(115200);
        delay(100);
        for (int i = 0; i < 100; i++) //Set initial RC time constant as the zero sensor
                level
                {
                        zero_cal += Get_RC_Time(Excitor_pin, Sense_pin);
                }
                zero_cal = zero_cal / 100;
}

void loop() {
        // put your main code here, to run repeatedly:
        static unsigned int measurement = 0;
        measurement = 0;
        for (int i = 0; i < sensor_averaging; i++)
        {
                measurement = measurement + Get_RC_Time(Excitor_pin, Sense_pin);
        }
        measurement = measurement / sensor_averaging;
        tone(speaker_pin,map(measurement, zero_cal, time_out, 60, 2000));
        Serial.print(zero_cal);
        Serial.print(", ");
        Serial.print(time_out);
        Serial.print(", ");
        Serial.println(measurement);
        if (measurement > touch_threshold) //This converts the sensor to an on/off (digital) switch
        {
                digitalWrite(LED_pin, HIGH);
        }
        else {
                digitalWrite(LED_pin, LOW);
        }
}

unsigned int Get_RC_Time(int e_pin, int s_pin)
{
        static unsigned int time_count = 0;
        time_count = 0;
        noInterrupts(); // critical, time-sensitive code here
        digitalWrite(e_pin, HIGH);
        for (unsigned int i = 0; i < time_out; i++)
        {
                if (!digitalRead(s_pin))
                {
                        time_count = time_count + 1; //integrate over the sample window
                }
        }
        digitalWrite(e_pin, LOW);
        pinMode(Sense_pin, OUTPUT);
        digitalWrite(Sense_pin, LOW); //Bringing the sence pin low enables a discharge faster than the RC time constant
        interrupts(); // Re enable inturrupts
        delay(1); //discharge delay
        pinMode(Sense_pin, INPUT);
        return (time_count);
}
