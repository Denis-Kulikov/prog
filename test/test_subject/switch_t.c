int Moving(int size, int Dice[size][size], int Direction, int Position[])
{
	switch (Direction) {
		case UP:
			if (Position[0] == 0)
				return FALSE;
			SWAP(Dice[Position[0] - 1][Position[1]], Dice[Position[0]][Position[1]]);
			Position[0]--;
			break;
		case DOWN:
			if (Position[0] == size - 1)
				return FALSE;
			SWAP(Dice[Position[0] + 1][Position[1]], Dice[Position[0]][Position[1]]);
			Position[0]++;
			break;
		case LEFT:
			if (Position[1] == 0)
				return FALSE;
			SWAP(Dice[Position[0]][Position[1] - 1], Dice[Position[0]][Position[1]]);
			Position[1]--;
			break;
		case RIGHT:
			if (Position[1] == size - 1)
				return FALSE;
			SWAP(Dice[Position[0]][Position[1] + 1], Dice[Position[0]][Position[1]]);
			Position[1]++;
			break;
	}
	return TRUE;
}