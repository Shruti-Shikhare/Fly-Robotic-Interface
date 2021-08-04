
IntervalTimer mytimer;


int R_input=0;
int L_input=0;


int previous_R_input=0;
int previous_L_input=0;

int voltage_threshold= 839;    // (2.7)*(1024/3.3 ) 

int spike_threshold=16;             

           
char R_window[1000]={0};           //window 1000 elements therefore 0.05 s wide
char L_window[1000]={0};

int R_window_sum=0;          //freq = window_sum /window_width(0.05)
int L_window_sum=0;

int i=0;


void setup() {
  
  pinMode(23,INPUT);
  pinMode(22,INPUT);

  pinMode(21,OUTPUT);    //Left Output
  pinMode(20,OUTPUT);    //Right output
  
 
  pinMode(13,OUTPUT);        //Need to set the MBED output pins

  mytimer.begin(get_signal,50);        //gets signal every 50 microseconds

}

void get_signal(){

  R_input=analogRead(22);       //Right cell signal
  L_input=analogRead(23);       //left cell signal
 
  char R=0;
  char L=0;


  if(R_input >= voltage_threshold && previous_R_input < voltage_threshold){  //Check if AP's have occured

  R=1;

  }

  else if(L_input >= voltage_threshold && previous_L_input < voltage_threshold) {
    
   
   L=1;
   
  }

  else if (R_input < voltage_threshold && previous_R_input > voltage_threshold) {

    R=0;
 
  }

  else if (L_input < voltage_threshold && previous_L_input > voltage_threshold) {

    L=0;
  }
  

 
  R_window[i]=R;                             //Assign AP's to rolling windows
  L_window[i]=L;
  
  previous_R_input=R_input;           
  previous_L_input=L_input;

    i++;                                
    i=i%1000;
}


void loop() {

  for(int j=0;j<=1000;j++){                 //Sum all AP's in the window        
    
    R_window_sum+=R_window[j];
    L_window_sum+=L_window[j];
       
  }

  

   if(R_window_sum > spike_threshold){        //Check if #AP's exceeds threshold

   digitalWrite(20,HIGH);                    //Output High to MBED IF so
   digitalWrite(13,HIGH); 

    
  }

  else if(L_window_sum > spike_threshold) {
   
    digitalWrite(21,HIGH);
    digitalWrite(13,HIGH);
    
  }

  else{
    
    digitalWrite(20,LOW);              //Output LOW to MBED IF not
    digitalWrite(21,LOW);
    digitalWrite(13,LOW);
    
    
  }
  
  R_window_sum=0;               //Reset Window Sums
  L_window_sum=0; 

}
