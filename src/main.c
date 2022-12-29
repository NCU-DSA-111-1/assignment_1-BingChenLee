#include "../inc/backprop.h"
#include "../inc/layer.h"
#include "../inc/neuron.h"

// Initial value
#define NUMBER_LAYERS 4
#define NUMBER_NEURON_INPUT_LAYER 2
#define NUMBER_NEURON_HIDDEN_LAYER1 4
#define NUMBER_NEURON_HIDDEN_LAYER2 4
#define NUMBER_NEURON_OUTPUT_LAYER 1
#define TRAINING_SETS 4
#define LEARNING_RATE 0.15
#define INPUT_LENGTH 0

enum state{
    SHOW_TRAINING = 1,
    HIDE_TRAINING = 2,
    TEST = 3
};

layer *lay = NULL;
int num_layers = 0;
int *num_neurons = NULL;
float learning_rate = 0;
float **training_input = NULL;
float **desired_outputs = NULL;
float sum_squ_error = 0;
float mse_total_error = 0;
int num_training_ex = 0;
int target_iteration_times = 0;
int round_count_max = 0;
int round_count = 0;
int state = TEST;
char *string = NULL;

FILE *error_analysis = NULL;

int main(void)
{
    srand(time(0));

    // Initialize the Value of the Variables
    data_initialize();

    // Get Training Examples
    get_training_inputs();
    
    // Get Desired Output
    get_desired_outputs();

    train_neural_net();
    
    test_nn();

    if(dinit()!= SUCCESS_DINIT)
    {
        printf("Error in Dinitialization...\n");
    }

    return 0;
}

void data_initialize()
{
    int i;

    // Initialize the Number of Layers (default 4)
    num_layers = NUMBER_LAYERS;

    num_neurons = (int*) malloc(num_layers * sizeof(int));
    memset(num_neurons, 0, num_layers * sizeof(int));

    // Get the Number of Neurons per Layer (default 2 4 4 1)
    *(num_neurons+0) = NUMBER_NEURON_INPUT_LAYER;
    *(num_neurons+1) = NUMBER_NEURON_HIDDEN_LAYER1;
    *(num_neurons+2) = NUMBER_NEURON_HIDDEN_LAYER2;
    *(num_neurons+3) = NUMBER_NEURON_OUTPUT_LAYER;

    // Initialize the Neural Network Module
    if(init() != SUCCESS_INIT)
    {
        printf("Error in Initialization...\n");
        exit(0);
    }

    // Initialize the Learning Rate (default 0.15)
    learning_rate = LEARNING_RATE;
    
    // Set the Iteration (training) Times
    do{
        printf("Enter your target iteration times: ");
        scanf("%d", &target_iteration_times);
        printf("\n");
    }while(target_iteration_times <= 0);
    
    // Initialize the Number of Training Examples (default 4)
    num_training_ex = TRAINING_SETS;
    
    // Create the Dynamic Memory Allocation for the Training Examples
    training_input = (float**) malloc(num_training_ex * sizeof(float*));
    for(i=0;i<num_training_ex;i++)
    {
        *(training_input+i) = (float*) malloc( *num_neurons * sizeof(float));
    }

    desired_outputs = (float**) malloc(num_training_ex * sizeof(float*));
    for(i=0;i<num_training_ex;i++)
    {
        *(desired_outputs+i) = (float*) malloc( *(num_neurons+(num_layers-1)) * sizeof(float));
    }

    // Ask if the user want to show the iteraion process
    do{
        printf("Do you want to show the iteration process? Enter 1 if you want, and enter 2 if you don't.\n");
        scanf("%d", &state);
        printf("\n");
    }while(state != SHOW_TRAINING && state != HIDE_TRAINING);
    
}

// Check if Create Architecture Sucessfully
int init()
{
    if(create_architecture() != SUCCESS_CREATE_ARCHITECTURE)
    {
        printf("Error in creating architecture...\n");
        return ERR_INIT;
    }

    return SUCCESS_INIT;
}

// Get Training Inputs
void  get_training_inputs()
{
    // set1: training input 0,0 
    *(*(training_input+0)+0)=0; 
    *(*(training_input+0)+1)=0;

    // set2: training input 0,1
    *(*(training_input+1)+0)=0; 
    *(*(training_input+1)+1)=1;

    // set3: training input 1,0 
    *(*(training_input+2)+0)=1; 
    *(*(training_input+2)+1)=0;

    // set4: training input 1,1
    *(*(training_input+3)+0)=1; 
    *(*(training_input+3)+1)=1;
}

// Get Desired Outputs
void get_desired_outputs()
{
    *(*(desired_outputs+0)+0)=0; // desired_output for inputs 0,0
    *(*(desired_outputs+1)+0)=1; // desired_output for inputs 0,1
    *(*(desired_outputs+2)+0)=1; // desired_output for inputs 1,0
    *(*(desired_outputs+3)+0)=0; // desired_output for inputs 1,1
}

// Feed Inputs to the Input Layer
void feed_input(int i)
{
    int j;

    for(j=0; j< *num_neurons ; j++)
    {
        lay[0].neu[j].actv = training_input[i][j];

        if(state == SHOW_TRAINING)
        {
            printf("%d", (int)lay[0].neu[j].actv); // Show Actual Output
        }
        
    }

    if(state == SHOW_TRAINING)
    {
        printf("/");
    }
    
}

// Create Neural Network Architecture
int create_architecture()
{
    int i=0,j=0;
    lay = (layer*) malloc(num_layers * sizeof(layer));

    for(i=0;i<num_layers;i++)
    {
        lay[i] = create_layer(*(num_neurons+i));      
        lay[i].num_neu = *(num_neurons+i);

        for(j=0; j< *(num_neurons+i) ; j++)
        {
            if(i < (num_layers-1)) 
            {
                lay[i].neu[j] = create_neuron( *(num_neurons+(i+1)) );
            }
        }
    }

    // Initialize the Weights
    if(initialize_weights() != SUCCESS_INIT_WEIGHTS)
    {
        printf("Error Initilizing weights...\n");
        return ERR_CREATE_ARCHITECTURE;
    }

    return SUCCESS_CREATE_ARCHITECTURE;
}

int initialize_weights(void)
{
    int i,j,k;

    if(lay == NULL)
    {
        printf("No layers in Neural Network...\n");
        return ERR_INIT_WEIGHTS;
    }

    for(i=0; i<num_layers-1; i++)
    {
        
        for(j=0; j< *(num_neurons+i) ; j++)
        {
            for(k=0; k< *(num_neurons+(i+1)) ; k++)
            {
                // Initialize Output Weights for each neuron
                lay[i].neu[j].out_weights[k] = ((double)rand())/((double)RAND_MAX);
                lay[i].neu[j].dw[k] = 0.0;
            }

            if(i>0) 
            {
                lay[i].neu[j].bias = ((double)rand())/((double)RAND_MAX);
            }
        }
    }   
    printf("\n");
    
    for (j=0; j<num_neurons[num_layers-1]; j++)
    {
        lay[num_layers-1].neu[j].bias = ((double)rand())/((double)RAND_MAX);
    }

    return SUCCESS_INIT_WEIGHTS;
}

// Train Neural Network
void train_neural_net(void)
{
    int i;
    int current_iteration_times=0;

    // Create a file (.csv) to store the analysis data
    if((error_analysis = fopen("error_analysis.csv","w+"))!=NULL)
    {
        fseek(error_analysis, 0, SEEK_SET);

        // Gradient Descent
        for(current_iteration_times=0; current_iteration_times<target_iteration_times; current_iteration_times++)
        {
            sum_squ_error=0;

            if(state == SHOW_TRAINING)
            {
                printf("Iteration Times: %-6d  ", current_iteration_times+1);
                printf("Inputs/Output: ");
            }
            
            for(i=0; i<num_training_ex; i++)
            {
                round_count_max = 1;
                round_count = 1;
                feed_input(i);
                forward_prop();
                compute_error(i);
                back_prop(i);
                update_weights();
            }

            mse_total_error += sum_squ_error/num_training_ex;
            
            if(state == SHOW_TRAINING)
            {
                printf("Error: %f  MSE: %f\n", sum_squ_error, mse_total_error/(current_iteration_times+1));
            }
            
            fprintf(error_analysis, "Iteration Times, %d, Error, %f, MSE, %f\n", current_iteration_times+1, sum_squ_error, mse_total_error/(current_iteration_times+1));
        }

        fclose(error_analysis);
    }

    printf("\nIterate successfully...\n");
    printf("Final MSE: %f\n", mse_total_error/target_iteration_times);
    printf("Now, you can enter your own test inputs (2~100 bits binary string, no space in the string)\n");
}

// Compute the Sum of Square Error
void compute_error(int i)
{
    int j;

    for(j=0; j< *(num_neurons+(num_layers-1)) ; j++)
    {
        sum_squ_error += (pow((desired_outputs[i][j] - lay[num_layers-1].neu[j].actv), 2));
    }   
}


// Back Propogate Error
void back_prop(int p)
{
    int i,j,k;

    // Output Layer
    for(j=0; j< *(num_neurons+(num_layers-1)); j++)
    {           
        lay[num_layers-1].neu[j].dz = (lay[num_layers-1].neu[j].actv - desired_outputs[p][j]) * (lay[num_layers-1].neu[j].actv) * (1- lay[num_layers-1].neu[j].actv);

        for(k=0; k< *(num_neurons+(num_layers-2)) ; k++)
        {   
            lay[num_layers-2].neu[k].dw[j] = (lay[num_layers-1].neu[j].dz * lay[num_layers-2].neu[k].actv);
            lay[num_layers-2].neu[k].dactv = lay[num_layers-2].neu[k].out_weights[j] * lay[num_layers-1].neu[j].dz;
        }
            
        lay[num_layers-1].neu[j].dbias = lay[num_layers-1].neu[j].dz;           
    }

    // Hidden Layers
    for(i=num_layers-2; i>0; i--)
    {
        for(j=0; j< *(num_neurons+i) ; j++)
        {
            if(lay[i].neu[j].z >= 0)
            {
                lay[i].neu[j].dz = lay[i].neu[j].dactv;
            }
            else
            {
                lay[i].neu[j].dz = 0;
            }

            for(k=0; k< *(num_neurons+(i-1)) ; k++)
            {
                lay[i-1].neu[k].dw[j] = lay[i].neu[j].dz * lay[i-1].neu[k].actv;    
                
                if(i>1)
                {
                    lay[i-1].neu[k].dactv = lay[i-1].neu[k].out_weights[j] * lay[i].neu[j].dz;
                }
            }

            lay[i].neu[j].dbias = lay[i].neu[j].dz;
        }
    }
}

void update_weights(void)
{
    int i,j,k;

    for(i=0; i<num_layers-1; i++)
    {
        for(j=0; j< *(num_neurons+i) ; j++)
        {
            for(k=0; k< *(num_neurons+(i+1)) ; k++)
            {
                // Update Weights
                lay[i].neu[j].out_weights[k] = (lay[i].neu[j].out_weights[k]) - (learning_rate * lay[i].neu[j].dw[k]);
            }
            
            // Update Bias
            lay[i].neu[j].bias = lay[i].neu[j].bias - (learning_rate * lay[i].neu[j].dbias);
        }
    }   
}

// Test the Trained Network
void test_nn(void) 
{

    while(1)
    {
        round_count = 1;
        state = TEST;

        // Create the Dynamic Memory Allocation for the string (input)
        string = (char*) malloc((INPUT_LENGTH) * sizeof(char));
        printf("\nEnter input to test: ");
        scanf("%s", string);
        
        // Check if user's inputs are in the correct form (2 bits or more)
        if(strlen(string) < 2)
        {
            printf("Please enter a binary string with at least 2 bits\n");
            free(string); // Free the memroies
            test_nn(); // Rerun the test to get new inputs
        }

        else
        {
            // Calculate how many rounds of XOR it will do. Ex: Input is 3 bits, it'll calculate 2 rounds
            round_count_max = strlen(string)-1;

            // Change the type from character to int
            lay[0].neu[0].actv = (*string)-'0'; 
            lay[0].neu[1].actv = (*(string+1))-'0'; 

            // Check if the user's inputs are in the correct form (only 0 and 1 in the string)
            if((lay[0].neu[0].actv != 0 && lay[0].neu[0].actv != 1) || (lay[0].neu[1].actv != 0 && lay[0].neu[1].actv != 1) )
            {
                printf("Please enter a binary string\n");
                free(string); // Free the memroies
                test_nn(); // Rerun the test to get new inputs
            }

            else
            {
                forward_prop(); // Test begin
                free(string); // Free the memroies
            }
        }
        
    }
}

void forward_prop(void)
{
    int i,j,k;

    for(i=1; i<num_layers; i++)
    {   
        for(j=0; j< *(num_neurons+i) ; j++)
        {
            lay[i].neu[j].z = lay[i].neu[j].bias;

            for(k=0; k< *(num_neurons+(i-1)) ; k++)
            {
                lay[i].neu[j].z  = lay[i].neu[j].z + ((lay[i-1].neu[k].out_weights[j]) * (lay[i-1].neu[k].actv));
            }

            // Relu Activation Function for Hidden Layers
            if(i < num_layers-1)
            {
                if((lay[i].neu[j].z) < 0)
                {
                    lay[i].neu[j].actv = 0;
                }

                else
                {
                    lay[i].neu[j].actv = lay[i].neu[j].z;
                }
            }
            
            // Sigmoid Activation function for Output Layer
            else
            {
                lay[i].neu[j].actv = 1/(1+exp(-lay[i].neu[j].z));
                
                // Check if it is the last round of the test
                if(round_count < round_count_max) // Not the last round
                {
                    lay[0].neu[0].actv = (int)round(lay[num_layers-1].neu[0].actv); // Set lay[0].neu[0].actv as the current round output value
                    lay[0].neu[1].actv = *(string+(round_count+1))-'0'; // Set lay[0].neu[1].actv as the next round new bit
                    round_count++;

                    // Detect the string is not in the binary form
                    if(lay[0].neu[1].actv != 0 && lay[0].neu[1].actv != 1)
                    {
                        printf("Please enter a binary string\n");
                        free(string); // Free the memroies
                        test_nn(); // Rerun the test to get new inputs
                    }

                    else
                    {
                        forward_prop(); // Keep testing the next bit
                    }
                }

                else // The end of training or test
                {   
                    switch (state)
                    {   
                        case SHOW_TRAINING:
                            printf("%d  ", (int)round(lay[num_layers-1].neu[0].actv)); // Print the output value
                            break;

                        case HIDE_TRAINING:
                            break;

                        case TEST:
                            printf("Output: %d\n", (int)round(lay[num_layers-1].neu[0].actv)); // Print the output value
                            break;
                    }
                    
                }
            }
        }
    }
}

int dinit(void)
{
    // Free the memroies
    free(training_input);
    free(desired_outputs);
    free(num_neurons);
    free(lay);
    return SUCCESS_DINIT;
}
