#include<stdio.h>
#include<conio.h>
#include<string.h>
int i =0;
int j=0;
int sum;
int idx = 0;
char c, n[10];
unsigned int str;
float hit=0;
float accessed=0;
int counter=0;
int read_values    =    300;
int cachesize,blocksize,ass;
int misstate=1;
int num[3];
int nofsets,tag,offset;
int physaddr=32;
char input_filename[250],input_filename1[250];


main()
{
FILE *fp1,*fp2;
int index;
printf("Enter the path of the configuration file \n");
scanf("%s",input_filename);
fp1=fopen("example.cfg","r");

if(fp1==NULL)
{
    printf("Error Opening file");
    goto error;
}
else
{
    while(!feof(fp1))
    {



        c=getc(fp1);
        if(isdigit(c))
        {
            while(1)
        {
            if(isdigit(c))
                n[i++]    =    c;
            else
            {
                num[idx]    =    atoi(n);
              //rintf("num[%d]=%d \n",idx,num[idx]);
                idx++;    //used for knowing which line.
                i = 0;    //reset for next line iteration.
                memset(n,0,10);
                break;
            }
            c=getc(fp1);
        }
    }

}

}
fp2=fopen("output.txt","w");

cachesize=num[0];
blocksize=num[1];
ass=num[2];
nofsets=cachesize/(blocksize*ass);
printf("\t \t \t  Cache Configuration \n");
printf("The number of sets is %d \n",nofsets);
//to find the offset bits
//And operator functionality
for(j=0;j<32;j++)
{
    if(blocksize & 1<<j)
{
 offset=j;
printf("The  number of offset bits is %d \n",offset);
}
}

//to find the number of index bits
for(j=0;j<32;j++)
{
    if(nofsets & 1<<j)
{
 index=j;
printf("The  number of index bits is %d \n",index);
}
}
tag=physaddr-index-offset;
sum=index+offset;
printf("The tag length is %d \n",tag);


FILE *fp3;
printf("Enter the path of the trace file to find the hit rate");
scanf("%s",input_filename1);
fp3=fopen("example.trc","r");
if(fp3==NULL)
{
    printf("Error Opening File");
    goto error;
}
else
{
    //to find which index to access compare the memory address with the index bit
    struct num
    {
        int block_tag[ass];
        int timestamp[ass];
    }set[nofsets];

	int ind, tagg;
    int decremeant_state;
    for(i=0;i<nofsets;i++)
    {
        for(j=0;j<ass;j++)
        {
            set[i].block_tag[j]    =    0;
            set[i].timestamp[j]    =    j+1;
        }
    }
    while(!feof(fp3))
    {
       misstate=1;
	    fscanf(fp3,"%x",&str);
        printf("%x\n",str);
        while(getc(fp3)!='\n')
        {
        	if(feof(fp3))
        	break;
		}

        ind = ((str >> offset) & nofsets-1);
        //fprintf(fp2,"The number of index bits is     =    %d \n",ind);
        tagg = str >>sum ;
        //fprintf(fp2,"The number of tag bits is     =    %d \n",tagg);

	   for(i=0;i<ass;i++)
        {//Its a MISS!
        if(set[ind].block_tag[i]==tagg)
        {
        	misstate=0;
		decremeant_state=i;
		break;
		}


    }
    if(misstate)
		{
            //tagg is miss, enter value in timestamp=1 block
            for(i=0;i<ass;i++)
                if(set[ind].timestamp[i]    ==    1)
                    set[ind].block_tag[i]    =    tagg;

            //Now check conditions before decreamenting timestanp values.
            //case 1: decremeant timestamp values and maintain within <4 boundary
            for(i=0;i<ass;i++)
            {
                set[ind].timestamp[i]--;
                if(!set[ind].timestamp[i])
                    set[ind].timestamp[i]    =    ass;    //MRU Value.
            }
        }

        //It's a HIT!
        else
        {

			//Find which block matched!

            //No Need to decreament timestamp if hit is in MRU block!
            if(set[ind].timestamp[decremeant_state]   !=    ass)
            {
                for(i=0;i<ass;i++)
                {
                	if(set[ind].timestamp[i]>set[ind].timestamp[decremeant_state])
                	{
                	set[ind].timestamp[i]--;
					}
				}
            set[ind].timestamp[decremeant_state]=ass;
        }
            //if HIT block timestamp is 3 just exchange timestamp of 4 block!

            //Incremeant hit variable!
            hit++;

		}

        //Number of accessed blocks!
        accessed++;

        //Control file read upto read_values.
        counter++;
       //if(counter == read_values)
         // break;

        //getch();
    }
    printf("\t \t \t  Cache Configuration \n");
    printf("The number of sets is %d \n",nofsets);
    printf("The  number of offset bits is %d \n",offset);
    printf("The  number of index bits is %d \n",index);
    printf("The tag length is %d \n",tag);
	printf("The number of addresses accessed is %d",accessed);
	printf("Hit = %2f, Miss = %f\n", hit, accessed-hit);
    printf("Hit Rate = %2f\n", (hit/accessed)*100);
    printf("Miss Rate = %2f\n", (100-((hit/accessed)*100)));

}

printf("\t\t\tSimulation is complete. \n");
fclose(fp1);
fclose(fp2);
fclose(fp3);

error:
    getch();

return 0;
}
