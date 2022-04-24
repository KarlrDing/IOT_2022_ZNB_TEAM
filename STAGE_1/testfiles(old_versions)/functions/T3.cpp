int patient_detection_control(int state, bool b1, bool b2, int mag,bool inout,int m, int s){
	switch(state){
		case 0:
		int counter = 0;
		bool code_flag1 = false;
		bool code_flag2 = false;
		bool code_flag3 = false;
		analogWrite(light_pin, 0); 
		if (b1 == true || b2 == true)
		{
			counter = m;
			return 1;
		}
		else if(inout == true)//true is out
		{
			return 2;
		}
		else if(mag == HIGH)
		{
			counter = m;
			return 1;
		}
		break;
		//END of CASE 0
		case 1://put in code
		analogWrite(light_pin, 255);
		if (s == 0 && counter < wait_time)
		{
			counter = counter + 1;
		}
		else if(s == 0 && counter == wait_time)
		{
			return 3;
			break;
		}
		if (b1 == true)
		{
			code_flag1 = true;
			analogWrite(light_pin, 200);
		}
		if (b2 == true && code_flag1 == true)
		{
			code_flag1 = false;
			code_flag2 = true;
			analogWrite(light_pin, 150);
		}
		if (b1 == true && code_flag2 == true)
		{
			code_flag2 = false;
			code_flag3 = true;
			analogWrite(light_pin, 100);
		}
		if (b1 == true && code_flag3 == true)
		{
			code_flag3 = false;
			analogWrite(light_pin, 0);
			return 0;
		}
		break;
		//END of CASE 1
		case 2:
		if (mag == LOW)
		{
			light_spark()
			return 0;
			break;
		}
		//END of CASE 2
		case 3:
		if (mag == LOW)
		{
			light_spark()
			return 0;
			break;
		}
		//END of CASE 3

	}
}


void light_spark(){
	analogWrite(light_pin, 255);
	delay(200);
	analogWrite(light_pin, 255);
	delay(200);
	analogWrite(light_pin, 255);
	delay(200);
	analogWrite(light_pin, 0);
}

bool inout_control(int mag, bool hand){
	if (mag == false && hand == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int patient_detection_control(int state, bool b1, bool b2, int mag,bool inout,int m, int s,int counter, bool code_flag1,bool code_flag2, bool code_flag3){
  switch(state){
    case 0:
    {
      counter = 0;
      code_flag1 = false;
      code_flag2 = false;
      code_flag3 = false;
      analogWrite(light_pin, 0); 
      if (b1 == true || b2 == true)
      {
        counter = m;
        return 1;
      }
      else if(inout == true)//true is out
      {
        return 2;
      }
      else if(mag == HIGH)
      {
        counter = m;
        return 1;
      }
      break;
    }
    //END of CASE 0
    case 1://put in code
    {
      analogWrite(light_pin, 255);
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      if (b1 == true)
      {
        code_flag1 = true;
        analogWrite(light_pin, 200);
      }
      if (b2 == true && code_flag1 == true)
      {
        code_flag1 = false;
        code_flag2 = true;
        analogWrite(light_pin, 150);
      }
      if (b1 == true && code_flag2 == true)
      {
        code_flag2 = false;
        code_flag3 = true;
        analogWrite(light_pin, 100);
      }
      if (b1 == true && code_flag3 == true)
      {
        code_flag3 = false;
        analogWrite(light_pin, 0);
        return 0;
      }
      break;
    }
    //END of CASE 1
    case 2:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
    }
    //END of CASE 2
    case 3:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
    }
    //END of CASE 3

  }
}




int patient_detection_control(int state, bool b1, bool b2, int mag,bool inout,int m, int s,int counter){
  switch(state){
    case 0:
    {
      counter = 0;
      analogWrite(light_pin, 0); 
      if (b1 == true || b2 == true)
      {
        counter = m;
        return 1;
      }
      else if(inout == true)//true is out
      {
        return 2;
      }
      else if(mag == HIGH)
      {
        counter = m;
        return 1;
      }
      return state;
      break;
    }
    //END of CASE 0
    case 1://put in code
    {
      analogWrite(light_pin, 255);
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 200);
          return 6;
          break;
        }
      }
    }
    //END of CASE 1
    case 6:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == true)//b1
        {
          analogWrite(light_pin, 170);
          return 7;
          break;
        }
      }
    }
    //END of CASE 6
    case 7:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 140);
          return 8;
          break;
        }
      }
    }
    //END of CASE 7
    case 8:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b2 == true)//b2
        {
          analogWrite(light_pin, 110);
          return 9;
          break;
        }
      }
    }
    //END of CASE 8
    case 9:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 80);
          return 10;
          break;
        }
      }
    }
    //END of CASE 9
    case 10:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b2 == true)//b2
        {
          analogWrite(light_pin, 50);
          return 11;
          break;
        }
      }
    }
    //END of CASE 10
    case 11:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 20);
          return 12;
          break;
        }
      }
    }
    //END of CASE 11
    case 12:
    {
      if (s == 0 && counter < wait_time)
      {
        counter = counter + 1;
      }
      else if(s == 0 && counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
      	if (b1 == true)//b1
        {
          analogWrite(light_pin, 0);
          return 0;
          break;
        }
      }
    }
    //END of CASE 10
    case 2:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 2
    case 3:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 3

  }
}


int patient_detection_control(int state, bool b1, bool b2, int mag,bool inout,int m, int s,int* counter){
  switch(state){
    case 0:
    {
      *counter = 0;
      analogWrite(light_pin, 0); 
      if (b1 == true || b2 == true)
      {
        *counter = m;
        return 1;
      }
      else if(inout == true)//true is out
      {
        return 2;
      }
      else if(mag == HIGH)
      {
        *counter = m;
        return 1;
      }
      return state;
      break;
    }
    //END of CASE 0
    case 1://put in code
    {
      analogWrite(light_pin, 255);
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 200);
          return 6;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 1
    case 6:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == true)//b1
        {
          analogWrite(light_pin, 170);
          return 7;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 6
    case 7:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 140);
          return 8;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 7
    case 8:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b2 == true)//b2
        {
          analogWrite(light_pin, 110);
          return 9;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 8
    case 9:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 80);
          return 10;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 9
    case 10:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b2 == true)//b2
        {
          analogWrite(light_pin, 50);
          return 11;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 10
    case 11:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == false && b2 == false)
        {
          analogWrite(light_pin, 20);
          return 12;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 11
    case 12:
    {
      if (s == 0 && *counter < wait_time)
      {
        *counter = *counter + 1;
      }
      else if(s == 0 && *counter == wait_time)
      {
        return 3;
        break;
      }
      else
      {
        if (b1 == true)//b1
        {
          analogWrite(light_pin, 0);
          return 0;
          break;
        }
        else
        {
        	return state;
        	break;
        }
      }
    }
    //END of CASE 10
    case 2:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 2
    case 3:
    {
      if (mag == LOW)
      {
        light_spark();
        return 0;
        break;
      }
      return state;
      break;
    }
    //END of CASE 3
  }
}
