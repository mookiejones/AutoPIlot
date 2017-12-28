  void Get_GPRTE()
  
 {
     static int klast;
     static int kstart;
    j_MAX = 20;
    int waypoint_count;
    Parse_Sentence();
    for(int j = 0; j<Word_count; j++)
     {
      data_RTE[j] = data_IN[j];  //see void Parse Sentence     
     }
     
     Number_of_sentences = To_Integer(data_RTE[1]);
     Current_sentence_number = To_Integer(data_RTE[2]);
     
     //  put the sequential sentences into a unified array
     waypoint_count = Word_count-5;
     if(Current_sentence_number == 1)
       {
         kstart = 0;
         klast = waypoint_count;
       }
     else
       {
         kstart = klast ;
         klast = klast + waypoint_count;
       } // end else
       
    if(Current_sentence_number == Number_of_sentences) Number_of_waypoints = klast;  //  First 5 words each sentence are not waypoints
     
     for(int j = 5; j<Word_count; j++)  
       {    
        Route [kstart + j-5 ] = data_RTE[j];
       } 
   
      if(print_RTE) {PRINT_RTE();}
 }  // end get_GPRTE()
 
 /**************************************************************/
 
    void PRINT_RTE()
 {  
    Serial.println();
    Serial.println("---------------");   
    Serial.println("RTE DATA "); 
    Serial.print("Number of sentences "); Serial.println(Number_of_sentences);
    Serial.print("Current Sentence "); Serial.println(Current_sentence_number);
    Serial.print("Word Count "); Serial.println(Word_count);
     for(int j = 0; j<Word_count; j++)
      {   
         Serial.println(data_RTE[j]);
      }
    Serial.print("Number of Waypoints, "); Serial.println(Number_of_waypoints);
    Serial.print("Route Data Array  ");
    for(int j = 0; j< 13; j++)
    {
      Serial.print(Route[j]);
      Serial.print(", ");
    }
    if(Current_sentence_number == Number_of_sentences) Serial.println();
    
    
 }
