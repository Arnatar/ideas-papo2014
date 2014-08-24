// size of the field x/y
#define X_SIZE 8
// y-size will be x_size / num_ranks (per rank, thus globally X_SIZE = Y_SIZE)

// it's not deterministic that there are actually 12 ideas, because they can be 
// placed over one another in the creation phase
#define NUM_IDEAS 16

// the maximum value for an idea's attributes
#define IDEA_MAX 10
