void PrintDice(int size , int Dice[size][size]) {
for (int i = 0; i < size; i++)
printf("+-----");
printf("+\n");
for (int y = 0; y < size; y++) {
for (int x = 0; x < size; x++) {
if (Dice[y][x] < 10) {
printf("|  %d  ", Dice[y][x]);
} else {
printf("|  %d ", Dice[y][x]);
}
}
printf("|\n");
for (int i = 0; i < size; i++)
printf("+-----");
printf("+\n");
}
printf("\n");
}