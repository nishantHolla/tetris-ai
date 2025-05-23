#include "tetris.h"

int main(int argc, char *argv[]) {

  // Parse arguments

  const char* short_options = "nth";
  static struct option long_options[] = {
    {"no-ai", no_argument, 0, 'n'},
    {"train", no_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, short_options,
          long_options, &option_index)) != -1) {
    switch (c) {
      case 'n':
        AI_IS_PLAYING = 0;
        break;
      case 't':
        AI_IS_TRAINING = 1;
        break;
      case 'h':
        printf("%s", HELP_TEXT);
        return 0;
      case '?':
        break;
      default:
        abort();
    }
  }


  if (AI_IS_TRAINING) {
    FILE *train_params = fopen("tetris-ai-train-params.txt", "r");
    tet_TrainParameters param = (tet_TrainParameters) {
      .generation_count = 100,
      .population_size = 100,
      .games_per_chromosome = 10,
      .moves_per_game = 5000,
      .elitsm_rate = 0.2,
      .mutation_rate = 0.4
    };
    if (train_params) {
      char line[100];
      if (!fgets(line, 100, train_params)) {
        perror("Error: Failed to read train params file.\n");
        exit(1);
      };
      int32_t inputs_read = sscanf(line, "%d %d %d %d %lf %lf",
          &param.generation_count,
          &param.population_size,
          &param.games_per_chromosome,
          &param.moves_per_game,
          &param.elitsm_rate,
          &param.mutation_rate);
      if (inputs_read != 6) {
        perror("Error: Failed to parse train params.\n");
        fclose(train_params);
        exit(1);
      }
      fclose(train_params);
    }
    tet_ai_train(param);
    return 0;
  }

  tet_Chromosome selected_chromosome = tet_default_chromosome;
  if (AI_IS_PLAYING) {
    FILE *params_file = fopen("./tetris-ai-params.txt", "r");
    if (params_file) {
      printf("Reading custom parameters\n");
      char line[500];
      if (!fgets(line, 500, params_file)) {
        perror("Error: Failed to read params file.\n");
        exit(1);
      };
      int32_t inputs_read = sscanf(line, "%lf %hhd %lf %hhd %lf %hhd %lf %hhd %lf %hhd %lf %hhd %lf %hhd %lf %hhd",
          &(selected_chromosome.coeff_of_line_clear[0].magnitude),
          &(selected_chromosome.coeff_of_line_clear[0].is_positive),
          &(selected_chromosome.coeff_of_line_clear[1].magnitude),
          &(selected_chromosome.coeff_of_line_clear[1].is_positive),
          &(selected_chromosome.coeff_of_line_clear[2].magnitude),
          &(selected_chromosome.coeff_of_line_clear[2].is_positive),
          &(selected_chromosome.coeff_of_line_clear[3].magnitude),
          &(selected_chromosome.coeff_of_line_clear[3].is_positive),
          &(selected_chromosome.coeff_of_line_clear[4].magnitude),
          &(selected_chromosome.coeff_of_line_clear[4].is_positive),
          &(selected_chromosome.coeff_of_bumpiness.magnitude),
          &(selected_chromosome.coeff_of_bumpiness.is_positive),
          &(selected_chromosome.coeff_of_height.magnitude),
          &(selected_chromosome.coeff_of_height.is_positive),
          &(selected_chromosome.coeff_of_holes.magnitude),
          &(selected_chromosome.coeff_of_holes.is_positive));
      if (inputs_read != 16) {
        printf("Error: Failed to parse params file. %d\n%s\n", inputs_read, line);
        fclose(params_file);
        exit(1);
      }
      fclose(params_file);
    }
  }

  // Init game

  srand(time(NULL));
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Tetris AI");
  SetTargetFPS(60);

  tet_ui_init_fonts();
  tet_Game game;
  if (tet_game_init(&game, &selected_chromosome) != 0) {
    perror("Failed to initialize game.\n");
    exit(1);
  }

  double last_call_time = 0;
  const double last_call_interval = AI_IS_PLAYING ? 0.1 : 0.3;

  // Game loop

  while (!WindowShouldClose()) {

    double current_time = GetTime();
    if (!game.is_over && current_time - last_call_time >= last_call_interval) {
      if (AI_IS_PLAYING) {
        tet_game_play(&game);
      }
      else if (tet_game_move(&game, TET_MOVE_DOWN) != 0) {
        tet_game_place(&game);
        tet_game_end_turn(&game);
      }
      last_call_time = current_time;
    }

    if (!AI_IS_PLAYING && !game.is_over) {
      if (IsKeyPressed(KEY_LEFT)) {
        tet_game_move(&game, TET_MOVE_LEFT);
      }
      else if (IsKeyPressed(KEY_RIGHT)) {
        tet_game_move(&game, TET_MOVE_RIGHT);
      }
      else if (IsKeyPressed(KEY_UP)) {
        tet_game_move(&game, TET_MOVE_ANTI_CLOCKWISE);
      }
      else if (IsKeyPressed(KEY_DOWN)) {
        tet_game_move(&game, TET_MOVE_CLOCKWISE);
      }
      else if (IsKeyPressed(KEY_SPACE)) {
        tet_game_move(&game, TET_MOVE_DROP);
        tet_game_place(&game);
        tet_game_end_turn(&game);
      }
      else if (IsKeyPressed(KEY_F)) {
        tet_game_move(&game, TET_MOVE_SWAP);
      }
    }

    if (game.is_over) {
      if (IsKeyPressed(KEY_R)) {
        tet_game_init(&game, &selected_chromosome);
        continue;
      }
    }

    BeginDrawing();

    ClearBackground(TET_UI_WINDOW_BG_COLOR);
    tet_ui_calculate(&game);
    tet_ui_draw_text(&game);
    tet_ui_draw_board(&game);
    tet_ui_draw_next_and_held_piece(&game);

    EndDrawing();
  }

  // Clean game

  CloseWindow();
  tet_ui_free_fonts();
  return 0;
}
