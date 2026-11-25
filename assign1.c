#include <stdio.h>
#define K ((1%7)+1) // Group Number 1

static int Critical3times = 0;
int Critical_Counter = 0, Warning_Counter = 0, Normal_Counter = 0, Check_Counter = 0, Invalid_Counter = 0;
float Voltage_Total = 0.0;

//self-def functions
void print_thresholds(void); //to print thresholds
void session_summary(void);// to view session summary
int check_valid (float,float); // to check vailidity
int Check_state(float,float); // to check current state for critical, normal, warning conditions

int main (void){
    float Voltage, Temperature;
    int Input, In_program = 0;

while(1){
    printf("MAIN MENU\n");
    printf("Input 1 to start monitoring\n");
    printf("Input 2 to show latest session summary\n");
    printf("Input 3 to view show group thresholds\n");
    printf("Input 4 to EXIT\n");

    scanf("%d", &Input);

    if (Input == 1){
        In_program = 1;
        Invalid_Counter = 0;
        Voltage_Total = 0.0; Critical3times = 0, Critical_Counter = 0, Warning_Counter = 0, Normal_Counter = 0, Check_Counter = 0;
        // initialise counter
    }
    else if (Input == 4){
        printf("Exiting program\n");
        break;
    }
    else if (Input == 2){
        printf("Session summary\n");
        session_summary();
    }
    else if (Input == 3){
        print_thresholds();
    }

    else{
        printf("Invalid input\n");
    }

    while (In_program){
        if (Critical3times >= 3){
            printf("Emergency shutdown: Critical condition occurred 3 times. Returning to main menu.\n");
            In_program = 0;
            break;
        }
        printf("Input Voltage (Volts): \n");
        scanf("%f", &Voltage);

        if (Voltage == -1){
            In_program = 0;
            break;
        }

        printf("Input Temperature (Celcius): \n");
        scanf("%f", &Temperature);
        int Valid = check_valid(Voltage, Temperature);

        if (Valid){
            printf("Valid input\n");
            Voltage_Total += Voltage;
            printf("Voltage: %.2f V\nTemperature: %.2f C\n", Voltage, Temperature);
            Check_state(Voltage, Temperature);
        }
    }
}
    return 0;
}
int check_valid(float Voltage, float Temperature){
    if (Voltage == -1){
        printf ("Exit\n");
        return -1;
    }
    if ((Voltage <= 15.0 && Voltage >= 10.0) && (Temperature <= 80.0 && Temperature >= -10.0)){
        return 1;
    }
    else{
        printf("Invalid input\n");
        Invalid_Counter++;
        return 0;
    }
}

int Check_state(float Voltage, float Temperature){
    if (Voltage < 11.0 ||  Voltage > 14.8 ||Temperature >= (50.0 + K) || (Voltage > 14.4 && Temperature >= (40.0 + K))){
        printf("Critical Condition\n");
        Critical3times++;
        Critical_Counter++;
        Check_Counter++;
        printf("Action: Disconnect charger and load.\n");
    }
    else if (Voltage < 11.8 || Voltage > 14.4 || Temperature >= (40.0 + K)){
        printf("Warning Condition\n");
        printf("Action: Reduce charging current; alert operator.\n");
        Critical3times = 0;
        Warning_Counter++;
        Check_Counter++;
    }
    else{
        printf("Normal Condition\n");
        printf("Action: Continue charging.\n");
        Critical3times = 0;
        Normal_Counter++;
        Check_Counter++;
    }
    return 0;
}

void session_summary(void){
    printf("Critical condition occurred %d times\n", Critical_Counter);
    printf("Warning condition occurred %d times\n", Warning_Counter);
    printf("Normal condition occurred %d times\n", Normal_Counter);
    printf("Total checks performed %d times\n", Check_Counter);
    printf("Invalid inputs received %d times\n", Invalid_Counter);
    if (Check_Counter > 0){
        printf("Average voltage: %.2f V\n", Voltage_Total / Check_Counter);
        printf("Critical percentage: %.2f%%\n", (float)Critical_Counter * 100.0 / Check_Counter);
    }
    else{
        printf("No valid readings; average voltage and critical percentage are unavailable.\n");
    }
}

void print_thresholds(void){
    printf("Thresholds:\n");

    printf("Critical Threshold: \n"
           "            Voltage < 11.00 V\n"
           "            Voltage > 14.80 V\n"
           "            Temperature >= %.2f C\n"
           "            Voltage > 14.40 V && Temperature >= %.2f C\n", 50.0 + K, 40.0 + K);

    printf("Warning Threshold: \n"
           "            Voltage < 11.80 V\n"
           "            Voltage > 14.40 V\n"
           "            Temperature >= %.2f C\n", 40.0 + K);

    printf("Normal Threshold: \n"
           "            11.80 V <= Voltage <= 14.40 V\n"
           "            Temperature < %.2f C\n", 40.0 + K);

    printf("K = %d\n", K);
    printf("Note: K is calculated as ((1%%7)+1)\n\n");
}
