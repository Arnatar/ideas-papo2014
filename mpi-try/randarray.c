double *createdata(int num)
{
     int i = 0;

     double *ptr;

     ptr = (double *)malloc(sizeof(double)*num);     

     if(ptr != NULL)
     {
       for(i = 0; i < num; i++)
       {
             ptr[i] = 0.0;       
       }
     } 
     return ptr; // return the pointer
  }  

double *gendata(int num) // you forgot the '*' here
{

     double *ptr = createdata(num); 

     int j = 0;

     for(j = 0; j < num; j++)
       {
             ptr[j] = rand();       
       }     
     return ptr; // i added this so you obviously return the pointer 
  }

int main()
{
    int c;
    double *data = gendata(8);
    for(c = 0; c < 8; c++)
    {
        printf(data[c]); 
    }
}
