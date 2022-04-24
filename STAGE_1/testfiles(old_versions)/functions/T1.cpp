bool button_state(bool value){
  bool state = false;
  if (value == true)
  {
    state = true;
  }
  return state;
}


bool bottle_state_control(bool state, bool value){
  if ((!state and value) == true)
  {
    return true;
  }
  else
  {
    red_light_alert();
  }
}

void red_light_alert(){
  digitalWrite(red_led_pin,HIGH);
  delay(1000);
  digitalWrite(red_led_pin,LOW);
  delay(1000);
}


int bottle_state_control(int state,bool button, bool time_up, bool value){
  switch(state){
    case 0:
    if (value == false)
    {
      return 5;//forgot to put the medicine bottle back to its position
    }
    else{
      return 1;//normal
    }
    break;
    //END of CASE 0
    case 1:
    if (time_up == true)
    {
      return 7;//forgot to take medicine
    }
    else if (value == false)
    {
      return 2;//the bottle has been taken up at least once
    }
    break;
    //END of CASE 1
    case 2:
    if (time_up == true)
    {
      return 7;//forgot to take medicine
    }
    else if (value == true)
    {
      return 3;//the bottle has been returned after taken
    }
    break;
    //END of CASE 2
    case 3:
    if (time_up == true)
    {
      return 7;//forgot to take medicine
    }
    else if (button == true)
    {
      return 6;//medicine has been taken
    }
    break;
    //END of CASE 3
    case 5:
    if (value == true)
    {
      return 3;//medicine bottle has been returned
    }
    break;
    //END of CASE 5
    case 7:
    if (button == true)
    {
      return 6;//medicine has been taken
    }
    break;
    //END of CASE 7
  }
  return state;
}

void light_control(int bottle){
  switch(bottle){
    case 5:
    red_light_alert();
    break;
    case 6:
    red_light_off();
    break;
    default:
    red_light_on();
  }
}

void buzzer_control(int bottle){
  if (bottle == 7)
  {
    
  }
}
