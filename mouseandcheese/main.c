#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int main() {
    //creating txt file for output
    FILE *fp;
    fp = fopen("outputfile.txt","w");
    if(fp== NULL){
        printf("\nFile cannot be opened!!\n");
        exit(1);
    }
    //creating an environment
  //  fputc('a',fp);
    int row;
    int column;
    //prompt the user to enter row and column values.
    printf("\nPlease enter row value : ");
    scanf("%d",&row);
    printf("\nPlease enter column value : ");
    scanf("%d",&column);

    int env[row][column];

    int q_cell = 0;
    for(int i = 0 ; i < row ; i++){
        for(int j = 0 ; j < column ; j++){
            env[i][j] = q_cell;
            q_cell++;
        }
    }
   /* for(int i = 0; i < row ; i++){
        for(int j = 0 ; j < column ; j++){
            printf("%d ",env[i][j]);
        } printf("\n");
    }*/
    int action[4] = {0,1,2,3}; //left,right,up,down respectively

    //determine the cheese state randomly.
    int coordinate_of_cheese;
    int boundary = row * column - 1;
    srand(time(NULL));
    int num = (rand() % boundary) + 1;

    coordinate_of_cheese = 15;

    //creating q_table
    int num_of_cell = row * column;
    double q_table[num_of_cell][4];
    for(int i = 0; i<num_of_cell; i++){
        for(int j = 0; j < 4 ; j++) {
            //all cells are filled by zero. Because firstly, all q-values are equal to zero.
            q_table[i][j] = 0.0;
           // printf("%f  ", q_table[i][j] );
        }
        //printf("\n");
    }

    //initialize all the parameters needed to implement the Q-Learning algorithm.
    int episodes = 10000;
    int max_step_per_episode = 3000;
    float learning_rate = 0.1;
    float discount_rate = 0.95;
    float exploration_rate = 0.25;
    float max_exploration_rate = 1;
    float min_exploration_rate = 0.01;
    float exploration_decay_rate = 0.000001;

    int rewards_all_episodes[episodes];
    //Q-Learning Algorithm
    int count_ep = 0;
    srand(time(NULL));
    for(int i = 0; i < episodes ; i++){

        int state = 0; // coordinate of mouse as agent
        short done = 0;
        int tot_rew = 0;
       // int previous_visit[max_step_per_episode];

        int stepnum = 0;
        for(int j = 0 ; j < max_step_per_episode ; j++){
            int rewards = 0;
            int action_per = 0;
            int max_q_action[2] = {INT_MIN,INT_MIN}; // to find a cell has a max q value, then action to the cell.
            //exploration-exploitation trade-off
            double exploration_rate_thershold = ((double)rand()/(RAND_MAX));
            if(exploration_rate_thershold > exploration_rate ){
                for(int k = 0 ; k < (sizeof(action)/sizeof(int)) ; k++){
                    if((k == 0 && (state % column != 0)) || (k == 1 && (state % column != (column - 1))) || (k==2 && (state > (column - 1))) || (k==3 && (state < column * (row-1)))){
                        if(max_q_action[1] <= q_table[state][k]){
                            max_q_action[0] = k;
                            max_q_action[1] = q_table[state][k];
                            action_per = k;

                        }
                    }
                }
            } else{
                int act[4];

                if(state % column == 0 && state < column){
                    act[0] = 1;
                    act[1] = 3;
                    int ran;
                    ran = rand() % 2 ;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                else if(state % column == 0 && state > (column * (row-1) -1)){
                    act[0] = 1;
                    act[1] = 2;
                    int ran;
                    ran = rand() % 2 ;
                    ran = act[ran];
                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                else if(state % column == column-1 && state < column){
                    act[0] = 0;
                    act[1] = 3;
                    int ran;
                    ran = rand() % 2 ;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                else if(state % column == column-1 && state > (column * (row-1) -1)){
                    act[0] = 0;
                    act[1] = 2;
                    int ran;
                    ran = rand() % 2 ;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                //far left side
                else if(state % column == 0){
                    for(int k = 0 ; k < 3 ; k++){
                        act[k] = k+1;
                    }
                    int ran ;
                     ran = rand() % 3;
                     ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                //far right side
                else if(state % column == column-1){
                    for(int k = 0 ; k < 3 ; k++){
                        act[k] = (k == 0 ? k : k+1);
                    }
                    int ran ;
                    ran = rand() % 3;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                //top side
                else if(state < column){
                    for(int k = 0 ; k < 3 ; k++){
                        act[k] = (k == 2 ? k+1 : k);
                    }
                    int ran ;
                     ran = rand() % 3;
                     ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                //lowest side
                else if(state > (column * (row-1) -1)){
                    for(int k = 0 ; k < 3 ; k++){
                        act[k] = k;
                    }
                    int ran ;
                    ran = rand() % 3;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
                else{
                    for(int k = 0 ; k < 4 ; k++){
                        act[k] = k;
                    }
                    int ran ;
                    ran = rand() % 4;
                    ran = act[ran];

                    max_q_action[0] = ran; max_q_action[1] = q_table[state][ran]; action_per = ran;
                }
            }
            int new_state;
            if(action_per == 0){
                new_state = state - 1;
            }else if(action_per == 1){
                new_state = state + 1;
            }else if(action_per == 2){
                new_state = state - column;
            }else{
                new_state = state + column;
            }

            if(new_state == coordinate_of_cheese){
                done = 1;
                rewards = 100;
            }
            else{
                rewards = -1;
            }
           // previous_visit[j+1] = new_state;
            //update Q-Table
            int max_q_action2[2] = {INT_MIN,INT_MIN};
            for(int k = 0 ; k < (sizeof(action)/sizeof(int)) ; k++){
                //if((k == 0 && (new_state % column != 0)) || (k == 1 && (new_state % column != column-1)) || (k==2 && (new_state > column-1)) || (k==3 && (new_state < (column*row)))){
                    if(max_q_action2[1] <= q_table[new_state][k]){
                        max_q_action2[0] = k;
                        max_q_action2[1] = q_table[new_state][k];
                    }
                //}
            }
            double q_val = q_table[state][action_per];
            double q_val2 = q_table[new_state][max_q_action2[0]];

            q_table[state][action_per] = (q_val* (1-learning_rate)) + (learning_rate * (rewards + (discount_rate * q_val2)));
          //  printf("* %d ",state);
            state = new_state;
            tot_rew += rewards;
            stepnum++;
            if(done==1)
                break;
            exploration_rate -= min_exploration_rate * (exploration_rate>0.01);

        }
        fprintf(fp,"\nnumber of steps: %d", stepnum);
        if(i == episodes -1 ){
            for(int m = 0 ; m < num_of_cell ; m++){
                fprintf(fp,"\n%d : {",m);
               for(int n = 0 ; n < 4 ; n++){
                    fprintf(fp,"%lf ,",q_table[m][n]);
                }
                fprintf(fp,"}");
            }
        }
       // printf("\n new episode");

       // printf("\n tot_rew = %d",tot_rew);
        rewards_all_episodes[i] = tot_rew;
    }
    fprintf(fp,"\nAverage reward per thousand episodes\n");
    double sum = 0;
    for(int j = 0 ; j < sizeof(rewards_all_episodes) / sizeof(int) ; j++){
        sum += rewards_all_episodes[j];
        if( j % 100 == 0){
            fprintf(fp,"\n%d : %f",j,(float)(sum / 100.0));
            sum = 0;
        }
        /*if(j <= 100)
            printf("\n%d : %f",j,rewards_all_episodes[j]);*/
        if(j == sizeof(rewards_all_episodes) / sizeof(int) - 1 )
            fprintf(fp,"\n%d : %f",j,rewards_all_episodes[j]);
    }
    fprintf(fp,"\n%d   \n", num);
    fclose(fp);
    return 0;
}

