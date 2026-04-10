bool piston_array[9]={false,false,false,false,false,false,false,false,false};
int last_piston_index = -1; // Variable to store the index of the last piston pushed

void piston_array_init()
{
  for(int i=0;i<9;i++)
  {
    piston_array[i]=false;
  }
  last_piston_index = -1; // Initialize the last piston index
}

void piston_array_set_top(int i)
{
    piston_array[i-1]=true;
}

void piston_array_set_bottom(int i)
{
  if(i<=9&&i>=1)
  {
    piston_array[i-1]=false;
  }
}

void check_and_push_piston()  //input 1 bis 9
{
  int new_i = random(1,10); //wert zwischen 1 9
//Serial.println("hier");
  // Generates new random value if the piston is already up or if it is the last piston pushed
  while (piston_array[new_i - 1]==true || new_i - 1 == last_piston_index) //array (true,false)
  { 
    Serial.println("hallo");
      new_i = random(1,10); //wert zwischen 1 9
  }
  int i = new_i;
  piston_array[i-1]=true;  //neue wert
  piston_array_set_bottom(i);
  push(i);

  last_piston_index = i - 1; // Update the last piston index
}