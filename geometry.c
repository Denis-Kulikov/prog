#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macro.h"

#define N 10 // максимальное количество фигур
#define L_CIRCLE 6
#define L_MAX 50
#define COORDINATES_X_Y 2
#define MAX_ITERATION 7

typedef struct figur_circle 
{
	double point[COORDINATES_X_Y];
	double radius;
}
StructCircle;

struct info 
{
	double perimeter;
	double area;
}
;

StructCircle InputArg()
{
	StructCircle Circle;
	int num = 0, arg = 0, space = 0, arg_coutn[] = {0, 0, 0},
is_minus[] = {0, 0, 0}, is_dot[] = {0, 0, 0};
	int unfinished = 0, minus_error = 0, dot_error = 0, void_arg_error = 0;
	char input = getchar();
	char buffer[L_MAX];
	for (int i = 0; i < L_MAX; i++)
		buffer[i] = 0;
	while (input != ')') {
		if (input == ')' && !arg_coutn[2])
			unfinished++;
		if (input == ' ')
			space++;
		if (input != ' ') {
			if (input == '-') {
				if ((is_minus[arg] || arg_coutn[arg]) && arg != 2) {
					minus_error++;
					for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
						printf(" ");
					}
					printf("^ неправильное использование минуса!\n");
					Circle.radius = -1;
					while (getchar() != '\n')
						;
					return Circle;
				}
				if (arg == 2) {
					minus_error++;
					for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
						printf(" ");
					}
					printf("^ радиус не может быть отрицательным\n");
					Circle.radius = -1;
					while (getchar() != '\n')
						;
					return Circle;
				}
				is_minus[arg]++;
			}
			else
			if (input == '.') {
				if (is_dot[arg]) {
					dot_error++;
					for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
						printf(" ");
					}
					printf("^ неправильное использование точки!\n");
					Circle.radius = -1;
					while (getchar() != '\n')
						;
					return Circle;
				}
				buffer[num] = '.';
				is_dot[arg]++;
				num++;
			}
			else
			if (input == ',') {
				if (arg_coutn[arg]) {
					buffer[num] = ' ';
					arg++;
					if (arg > 2) {
						for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
							printf(" ");
						}
						printf("^ Лишний токен\n");
						Circle.radius = -1;
						while (getchar() != '\n')
							;
						return Circle;
					}
				}
				else 
				{
					void_arg_error++;
					for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
						printf(" ");
					}
					printf("^ Пустой аргумент!\n");
					Circle.radius = -1;
					while (getchar() != '\n')
						return Circle;
				}
				num++;
			}
			else
			if (isdigit(input)) {
				arg_coutn[arg]++;
				buffer[num] = input;
				num++;
			}
			else 
			{
				if (input == '\n') {
					for (int i = 0; i < num + L_CIRCLE + 1 + space; i++) {
						printf(" ");
					}
					printf("^ Пустой токен!\n");
					Circle.radius = -1;
					return Circle;
				}
				printf("Неверно введены аргументы!\n");
				Circle.radius = -1;
				while (getchar() != '\n')
					;
				return Circle;
			}
		}
		input = getchar();
	}
	if (input == ')') {
		while (getchar() != '\n')
			;
		if (!minus_error && !dot_error && !void_arg_error && arg == 2) {
			char *YValue, *RadValue;
			Circle.point[0] = strtod(buffer, &YValue);
			if (is_minus[0])
				Circle.point[0] *= -1;
			Circle.point[1] = strtod(YValue, &RadValue);
			if (is_minus[1])
				Circle.point[1] *= -1;
			Circle.radius = strtod(RadValue, NULL);
			return Circle;
		}
		else 
		{
			printf("Параметры введены не верно.\n");
			Circle.radius = -1;
			return Circle;
		}
	}
	Circle.radius = -1;
	return Circle;
}

StructCircle InputInfo()
{
	StructCircle Circle;
	char figur_circle[] = "circle", figur_enter[L_MAX], answer = 0;
	int is_circle;

	int num = -1, space_error = 0, fatal_error = 0;
	do {
		num++;
		figur_enter[num] = getchar();
		COMPARISON(figur_enter, figur_circle, L_CIRCLE, is_circle);
		if (figur_enter[num] == '\n' && num > 0) {
			printf("^Неправильно введена фигура.\n");
			Circle.radius = -1;
			return Circle;
		}
		if (num > L_CIRCLE) {
			while (figur_enter[num] != '\n')
				figur_enter[num] = getchar();
			printf("^Неправильно введена фигура.\n");
			Circle.radius = -1;
			return Circle;
		}
		if (figur_enter[num] == '\n' && num == 0) {
			do {
				printf("Хотите закончить ввод данных? y/n \n");
				answer = getchar();
				if (answer != 'y' && answer != 'Y' && answer != 'n'
				&& answer != 'N') {
					while (getchar() != '\n')
						;
				}
			} while (answer != 'y' && answer != 'Y' && answer != 'n'
			&& answer != 'N');
			answer = tolower(answer);
			getchar();
			if (answer == 'y') {
				printf("Конец ввода.\n");
				Circle.radius = -2;
				return Circle;
			}
			num = -1;
		}
		if (is_circle && figur_enter[num] != '(' && num >= L_CIRCLE) {
			if (is_circle && figur_enter[num] == ' ') {
				space_error++;
				num--;
			}
			else 
			{
				fatal_error++;
			}
		}
		if (figur_enter[num] == '(')
			break;
	} while (figur_enter[num] != '(' && num < L_MAX);
	if (is_circle) {
		if (space_error)
			printf("       ^Лишний отступ перед аргументами.\n");
		if (fatal_error)
			printf("Фигура не распознана. Ожидается: %s\n", figur_circle);
		if (space_error || fatal_error) {
			Circle.radius = -1;
			return Circle;
		}
		if (!space_error && !fatal_error)
			return Circle = InputArg(Circle);
	}

	Circle.radius = -1;
	return Circle;
}

void arg_del(char* arg, int length)
{
	for (int element = 0; element < length; element++)
		arg[element] = '\0';
}

struct info fun_area_perimetr(StructCircle cir)
{
	struct info number;
	number.perimeter = 2 * M_PI * cir.radius;
	number.area = M_PI * cir.radius * cir.radius;

	return number;
}

int fun_check_intersection(StructCircle Acir, StructCircle Bcir)
{
	double temp,
	distance
	= (Acir.point[0] - Bcir.point[0]) * (Acir.point[0] - Bcir.point[0])
	+ (Acir.point[1] - Bcir.point[1]) * (Acir.point[1] - Bcir.point[1]);
	temp = distance;
	if (temp != 0) {
		for (int iteration = 0; iteration < MAX_ITERATION; iteration++)
			distance = (distance + temp / distance) / 2;
	}

	if (distance <= Acir.radius + Bcir.radius) {
		return 1;
	}
	return 0;
}

int main()
{
	int i, j, num;
	StructCircle circle_pos[N];
	struct info area_perimetr[N];

	printf("Уровень: Easy\nПравило ввода данных: тип(координата X, координата "
	"Y, радиус)\nПример: circle(1, 2, 3)\nОдна фигура занимает ровно "
	"одну строку\n"
	"Чтобы завершить ввод нажмите ещё раз enter после ввода данных\n\n");

	int info_intersection[N][N];
	for (int first_figur = 0; first_figur < N; first_figur++) {
		for (int second_figur = 0; second_figur < N; second_figur++) {
			info_intersection[first_figur][second_figur] = 0;
		}
	}

	for (i = 0; i < N; i++) {
		circle_pos[i] = InputInfo();
		if (circle_pos[i].radius == -2) {
			num = i;
			break;
		}
		if (circle_pos[i].radius == -1)
			i--;
		if (i == N - 1)
			num = i;
	}

	for (i = 0; i < num; i++) {
		for (j = 0; j < num; j++) {
			if (i == j) {
				j++;
				if (j == num)
					break;
			}
			info_intersection[i][j]
			= fun_check_intersection(circle_pos[i], circle_pos[j]);
		}
	}

	for (i = 0; i < num; i++) {
		area_perimetr[i] = fun_area_perimetr(circle_pos[i]);
	}

	for (i = 0; i < num; i++) {
		printf("%d. circle(%.2f %.2f %.2f)\n\tperimeter = %.3f\n\tarea = "
		"%.3f\n\tintersects:\n",
		i + 1,
		circle_pos[i].point[0],
		circle_pos[i].point[1],
		circle_pos[i].radius,
		area_perimetr[i].perimeter,
		area_perimetr[i].area);
		for (j = 0; j < num; j++) {
			if (i == j) {
				j++;
				if (j == num)
					break;
			}
			if (info_intersection[i][j] == 1)	
				printf("\t   %d. circle\n", j + 1);
		}
		printf("\n");
	}
	return 0;
}
