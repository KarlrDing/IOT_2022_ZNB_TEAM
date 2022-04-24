void env_init(){
  if (!ENV.begin())
  {//check correct initialisation of shield
    Serial.println("Failed to initialize MKR ENV shield!");
    while(1);//infinite loop, avoids loop to be executed
  }
}

bool door_state_detection(bool door, int light){
  if (light >= 200)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int clock_p(int counter){
  delay(500);
  if (counter == 1)
  {
    counter = 0;
  }
  else
  {
    counter = counter + 1;
  }
  return counter
}

int clock_s(int counter, int sec){
  if (counter == 1)
  {
    if (sec == 60)
    {
      sec = 0;
    }
    else
    {
      sec = sec + 1;
    }
  }
  return sec
}

bool fall_alarm_detect(int sound, int humidity, bool door){
  if (sound >= 100 && humidity >= 30 && door == true)//change threshold
  {
    return true;
  }
}

int fall_alarm_buzzer(bool flag, int counter, int minu, int sec){
  if (flag == false)
  {
    counter = minu;
  }
  else
  {
    if (sec == 0 && counter < wait_time)
    {
      counter = counter + 1;
    }
    else if (sec == 0 && counter == wait_time)
    {
      tone(buzzer, 1000);
      delay(500);
      noTone(buzzer);
      delay(500); 
      counter == 0;
    }
  }
  return counter;
}

bool door_close_detect(bool door){
  if (door == true)//change threshold
  {
    return true;
  }
  else
  {
    return false;
  }
}

int fall_alarm_monitor(int sound, float humidity, float temperature, bool flag, int counter, int sec, int minu){
  if (flag == false)
  {
    counter = minu;
  }
  else
  {
    if (sec == 0 && counter < wait_time)
    {
      counter = counter + 1;
    }
    else if (sec == 0 && counter == wait_time)
    {
      if (sound >= alarm_sound && humidity >= alarm_humidity && temperature >= alarm_temp)
      {
        tone(buzzer, 1000);
        delay(500);
        noTone(buzzer);
        delay(500);
      }
    }
  }
  return counter;
}

int sound_value_max(int max,int sound){
  if (sound >= max)
  {
    max = sound;
  }
  return max;
}
