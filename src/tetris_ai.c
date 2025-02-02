#include "tetris.h"

// AI Functions

int8_t tet_ai_init_population(tet_Chromosome *population, const tet_TrainParameters *train_param) {
  /*
     Initializes the given population with random chromosomes. Returns 0 if successful else returns
     non 0 integer
  */

  const int32_t POPULATION_SIZE = train_param->population_size;

  for (int32_t i = 0; i < POPULATION_SIZE; i++) {
    tet_ai_init_chromosome(&(population[i]));
  }

  return 0;
}

int8_t tet_ai_init_chromosome(tet_Chromosome *chromosome) {
  /*
     Initializes the given chromosome with random genes. Returns 0 if successful else returns non
     0 integer
  */

  for (int8_t i = 0; i < 5; i++) {
    chromosome->coeff_of_line_clear[i] = (tet_Coefficient) {
      .magnitude = tet_generate_random_double(0, MAX_COEFFICIENT_MAGNITUDE),
      .is_positive = tet_generate_random_int(0, 2)
    };
  }

  chromosome->coeff_of_holes = (tet_Coefficient) {
    .magnitude = tet_generate_random_double(0, MAX_COEFFICIENT_MAGNITUDE),
    .is_positive = tet_generate_random_int(0, 2)
  };

  chromosome->coeff_of_height = (tet_Coefficient) {
    .magnitude = tet_generate_random_double(0, MAX_COEFFICIENT_MAGNITUDE),
    .is_positive = tet_generate_random_int(0, 2)
  };

  chromosome->coeff_of_bumpiness = (tet_Coefficient) {
    .magnitude = tet_generate_random_double(0, MAX_COEFFICIENT_MAGNITUDE),
    .is_positive = tet_generate_random_int(0, 2)
  };

  chromosome->fitness = -1;
  return 0;
}

int compare(const void *a, const void *b) {
  return ((tet_Chromosome *)a)->fitness - ((tet_Chromosome *)b)->fitness;
}

int8_t tet_ai_train(tet_TrainParameters train_param) {
  /*
     Run genetic algorithm to determine best coefficients. Returns 0 if successful else returns
     non 0 integer
  */

  const int32_t POPULATION_SIZE = train_param.population_size;
  const int32_t GENERATION_COUNT = train_param.generation_count;

  printf("Training for %d generations with population size of %d\n", GENERATION_COUNT, POPULATION_SIZE);

  tet_Chromosome *population = (tet_Chromosome *) malloc(sizeof(tet_Chromosome) * train_param.population_size);
  if (!population) {
    perror("Error: Failed to allocate memory\n");
    exit(1);
  }

  char log_file_name[100];
  snprintf(log_file_name, 100, "tetris-ai-train-%lu.log", (unsigned long)time(NULL));

  if (tet_ai_init_population(population, &train_param) != 0) {
    printf("Failed to Initializes population\n");
    return 1;
  }
  else {
    printf("Initialized population\n");
  }

  for (int32_t gen_number = 0; gen_number < GENERATION_COUNT; gen_number++) {
    FILE *log_file = fopen(log_file_name, "a");

    fprintf(log_file, "Generation %d:\n", gen_number);
    printf("Generation %d: aaa", gen_number);
    fflush(stdout);

    for (int32_t i = 0; i < POPULATION_SIZE; i++) {
      printf("\b\b\b%03d", i);
      fflush(stdout);
      tet_ai_calculate_fitness(&(population[i]), &train_param);
    }

    tet_ai_select_and_crossover(population, &train_param);
    tet_ai_mutate(population, &train_param);

    tet_Chromosome best_chromosome = population[POPULATION_SIZE-1];
    printf(" Best fitness: %ld\n", best_chromosome.fitness);
    for (int32_t i = 0; i < 5; i++) {
      fprintf(log_file, "\tcoeff_of_lines_cleared %d: magnitude=%f, is_positive=%d\n", i,
          best_chromosome.coeff_of_line_clear[i].magnitude,
          best_chromosome.coeff_of_line_clear[i].is_positive);
    }

    fprintf(log_file, "\tcoeff_of_holes: magnitude=%f, is_positive=%d\n",
        best_chromosome.coeff_of_holes.magnitude,
        best_chromosome.coeff_of_holes.is_positive);

    fprintf(log_file, "\tcoeff_of_heights: magnitude=%f, is_positive=%d\n",
        best_chromosome.coeff_of_height.magnitude,
        best_chromosome.coeff_of_height.is_positive);

    fprintf(log_file, "\tcoeff_of_bumpiness: magnitude=%f, is_positive=%d\n",
        best_chromosome.coeff_of_bumpiness.magnitude,
        best_chromosome.coeff_of_bumpiness.is_positive);

    fclose(log_file);
  }

  tet_Chromosome best_chromosome = population[POPULATION_SIZE-1];
  FILE *params_file = fopen("tetris-ai-params.txt", "w");
  if (!params_file) {
    perror("Error: Failed to write parameters to tetris-ai-parms.txt\n");
    exit(1);
  }
  for (int32_t i = 0; i < 5; i++) {
    fprintf(params_file, "%.8f %d ", best_chromosome.coeff_of_line_clear[i].magnitude, best_chromosome.coeff_of_line_clear[i].is_positive);
  }
  fprintf(params_file, "%.8f %d ", best_chromosome.coeff_of_bumpiness.magnitude, best_chromosome.coeff_of_bumpiness.is_positive);
  fprintf(params_file, "%.8f %d ", best_chromosome.coeff_of_height.magnitude, best_chromosome.coeff_of_height.is_positive);
  fprintf(params_file, "%.8f %d ", best_chromosome.coeff_of_holes.magnitude, best_chromosome.coeff_of_holes.is_positive);
  fclose(params_file);
  free(population);
  return 0;
}

int8_t tet_ai_calculate_fitness(tet_Chromosome *chromosome, const tet_TrainParameters *train_param) {
  /*
     Calculates the fitness of the given chromosomes by playing some games with the genes of the
     the chromosome. Fills the fitness value of the given chromosome struct. Returns 0 if successful
     else return non 0 integer
  */

  const int32_t GAMES_PER_CHROMOSOME = train_param->games_per_chromosome;
  const int32_t MOVES_PER_GAME = train_param->moves_per_game;
  int64_t fitness_sum = 0;

  for (int32_t i = 0; i < GAMES_PER_CHROMOSOME; i++) {
    tet_Game game;
    tet_HashMap visited;

    if (tet_game_init(&game, chromosome) != 0) {
      printf("Failed to initialize game\n");
      return 1;
    }

    if (tet_hashmap_init(&visited) != 0) {
      printf("Failed to initialize hashmap\n");
      return 1;
    }

    int64_t moves = 0;
    while (!game.is_over && moves < MOVES_PER_GAME) {
      tet_game_play(&game);
      /*tet_debug_print_game(&game, true);*/
      moves++;
    }

    int64_t fitness = 0;
    fitness += game.score;
    fitness -= (game.l1_clear * 1);
    fitness -= (game.l2_clear * 2);
    fitness += (game.l3_clear * 3);
    fitness += (game.l4_clear * 4);

    fitness_sum += fitness;
    tet_hashmap_free(&visited);
  }

  chromosome->fitness = (fitness_sum / GAMES_PER_CHROMOSOME);
  return 0;
}

int8_t tet_ai_select_and_crossover(tet_Chromosome *population, const tet_TrainParameters *train_param) {
  /*
     Performs selection and crossover for the given population based on the set ELITISM_RATE.
     Returns 0 if successful else returns non 0 integer
  */

  const int32_t POPULATION_SIZE = train_param->population_size;
  const double ELITISM_RATE = train_param->elitsm_rate;


  qsort(population, POPULATION_SIZE, sizeof(tet_Chromosome), &compare);
  int32_t crossover_end = POPULATION_SIZE - (ELITISM_RATE * POPULATION_SIZE);

  for (int32_t i = 0; i < crossover_end; i++) {

    int32_t parent_a_index = tet_generate_random_int(crossover_end, POPULATION_SIZE);
    int32_t parent_b_index = parent_a_index;

    while (parent_a_index == parent_b_index) {
      parent_b_index = tet_generate_random_int(crossover_end, POPULATION_SIZE);
    }

    tet_Chromosome child = population[parent_a_index];

    for (int32_t j = 0; j < 5; j++) {
      if (tet_generate_random_int(0, 2) % 2 == 0) {
        child.coeff_of_line_clear[j] = population[parent_b_index].coeff_of_line_clear[j];
      }
    }

    if (tet_generate_random_int(0, 2) % 2 == 0) {
      child.coeff_of_bumpiness = population[parent_b_index].coeff_of_bumpiness;
    }

    if (tet_generate_random_int(0, 2) % 2 == 0) {
      child.coeff_of_holes = population[parent_b_index].coeff_of_holes;
    }

    if (tet_generate_random_int(0, 2) % 2 == 0) {
      child.coeff_of_height = population[parent_b_index].coeff_of_height;
    }

    population[i] = child;
  }

  return 0;
}

int8_t tet_ai_mutate(tet_Chromosome *population, const tet_TrainParameters *train_param) {
  /*
     Mutates the given population based on MUTATION_RATE. Returns 0 if successful else returns 0
  */

  const int32_t POPULATION_SIZE = train_param->population_size;
  const double ELITISM_RATE = train_param->elitsm_rate;
  const double MUTATION_RATE = train_param->mutation_rate;

  int32_t mutation_count = POPULATION_SIZE * MUTATION_RATE;
  int32_t crossover_end = POPULATION_SIZE - (ELITISM_RATE * POPULATION_SIZE);

  for (int32_t i = 0; i < mutation_count; i++) {
    int32_t mutating_index = tet_generate_random_int(0, crossover_end);
    double mutation;

    for (int32_t j = 0; j < 5; j++) {
      mutation = population[mutating_index].coeff_of_line_clear[j].magnitude + tet_generate_random_double(0, MAX_MUTATION);
      if (mutation > 0) {
        population[mutating_index].coeff_of_line_clear[j].magnitude = mutation;
      }
      population[mutating_index].coeff_of_line_clear[j].is_positive = tet_generate_random_int(0, 2);
    }

    mutation = population[mutating_index].coeff_of_height.magnitude + tet_generate_random_double(0, MAX_MUTATION);
    if (mutation > 0) {
      population[mutating_index].coeff_of_height.magnitude = mutation;
    }
    population[mutating_index].coeff_of_height.is_positive = tet_generate_random_int(0, 2);

    mutation = population[mutating_index].coeff_of_holes.magnitude + tet_generate_random_double(0, MAX_MUTATION);
    if (mutation > 0) {
      population[mutating_index].coeff_of_holes.magnitude = mutation;
    }
    population[mutating_index].coeff_of_holes.is_positive = tet_generate_random_int(0, 2);

    mutation = population[mutating_index].coeff_of_bumpiness.magnitude + tet_generate_random_double(0, MAX_MUTATION);
    if (mutation > 0) {
      population[mutating_index].coeff_of_bumpiness.magnitude = mutation;
    }
    population[mutating_index].coeff_of_bumpiness.is_positive = tet_generate_random_int(0, 2);

  }


  return 0;
}
