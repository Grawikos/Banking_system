#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT                                                                  \
	"\n%s, %s\nAddress %s\nAcc. number %d\nId '%s'\nBalance %d\nSavings "      \
	"%d\n\n"
#define PARAMETERS                                                             \
	acc.name, acc.surname, acc.address, acc.number, acc.id, acc.balance,       \
		acc.savings

typedef struct {
	char name[20];
	char surname[20];
	char address[20];
	char id[4];
	int number;
	int balance;
	int savings;
} account, *accountPtr;

void reset()
{
	system("clear");
	remove("accounts.bin");
	FILE* fp;
	fp = fopen("accounts.bin", "w");
	fclose(fp);
}

int inserted_id(char* id)
{
	int counter = -1;
	char next = getchar();
	while (next != '\n') {
		counter++;
		if (next < '0' || next > '9') {
			while (getchar() != '\n')
				;
			return 0;
		}
		if (counter >= 4) {
			while (getchar() != '\n')
				;
			return 0;
		}
		id[counter] = next;
		next = getchar();
	}
	if (counter < 3)
		return 0;
	return 1;
}

int input_id(char* id)
{
	printf("Input id\n>");
	while (!inserted_id(id)) {
		printf("Wrong format of id.\nTry again or type '0000' to cancel "
			   "account operation\n>");
	}
	if (!strncmp(id, "0000", 4)) {
		return 0;
	}
	return 1;
}

int confirm()
{
	int decision = -1;
	printf("1 - confirm your changes\n0 - abort changes");
	while (1) {
		printf("\n> ");
		if (scanf("%d", &decision)) {
			while (getchar() != '\n')
				;
			if (decision < 0 || decision > 1)
				continue;
			else
				break;
		}
		else {
			while (getchar() != '\n')
				;
			decision = -1;
		}
	}
	return decision;
}

FILE* open_accounts(const char* mode)
{
	FILE* file = fopen("accounts.bin", mode);
	if (file == NULL) {
		printf("Failed to open file for deposit.\n");
		exit(1);
	}
	return file;
}

FILE* open_tmp()
{
	FILE* fp_tmp = fopen("tmp.bin", "wb");
	if (!fp_tmp) {
		printf("Unable to open file temp file.");
		exit(1);
	}
	return fp_tmp;
}

int validity_of_trannsfer(int max)
{
	int number = 0, counter = 0;
	char next = getchar();
	int digit;
	while (next != '\n') {
		counter++;
		if (next < '0' || next > '9') {
			while (getchar() != '\n')
				;
			return -1;
		}
		digit = next - '0';
		if (max - digit < 0) {
			while (getchar() != '\n')
				;
			return -1;
		}
		if ((max - digit) / 10 < number) {
			while (getchar() != '\n')
				;
			return -1;
		}
		number = number * 10 + digit;
		next = getchar();
	}
	if (!counter)
		return -1;
	return number;
}

int compare(const void* first, const void* sec)
{
	return (*(int*)first - *(int*)sec);
}

int smallest_possible(int* numbers, int len)
{
	if (!len)
		return 1;
	int i;
	for (i = 1; i <= len; i++) {
		if (numbers[i - 1] != i)
			return i;
	}
	return i + 1;
}

int check_numbers()
{
	int counter = -1;
	int size = 4;
	int* tmp;
	int* numbers = malloc(size * sizeof(int));
	FILE* file = open_accounts("rb");
	if (NULL == numbers) {
		printf("Couldn't allocate memory");
		exit(0);
	}
	account acc;
	while (fread(&acc, sizeof(acc), 1, file) != 0) {
		counter++;
		if (counter - 1 >= size) {
			size *= 2;
			tmp = realloc(numbers, size * sizeof(int));
			if (NULL == tmp) {
				free(numbers);
				printf("Couldn't allocate memory");
				exit(0);
			}
			numbers = tmp;
		}
		numbers[counter] = acc.number;
	}
	qsort(numbers, counter + 1, sizeof(int), compare);
	int next = smallest_possible(numbers, counter + 1);
	free(numbers);
	fclose(file);
	return next;
}

void file_exchange(int found, char* id)
{
	if (!found)
		printf("No record(s) found with the requested id: %s\n\n", id);
	if (found && confirm()) {
		remove("accounts.bin");
		rename("tmp.bin", "accounts.bin");
	}
	else
		remove("tmp.bin");
}

int check_id(char* id)
{
	FILE* acc_file = open_accounts("rb");
	account acc;
	while (fread(&acc, sizeof(acc), 1, acc_file) != 0)
		if (!strncmp(acc.id, id, 4)){
			fclose(acc_file);
			return 0;
		}
	fclose(acc_file);
	return 1;
}

int inserted_text(char* name, const char* what)
{
	printf("\n%s> ", what);
	int counter = -1;
	char next = getchar();
	while (next != '\n') {
		counter++;
		if (!((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z'))) {
			while (getchar() != '\n')
				;
			return 0;
		}
		if (counter >= 20) {
			while (getchar() != '\n')
				;
			return 0;
		}
		name[counter] = next;
		next = getchar();
	}
	if (counter < 1)
		return 0;
	counter++;
	while(counter <= 19){
		name[counter] = '\0';
		counter++;
	}
	return 1;
}

int inserted_address(char* address)
{
	printf("\nAddress> ");
	int counter = -1;
	char next = getchar();
	while (next != '\n') {
		counter++;
		if (!((next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z') || next == ' ' || (next >= '0' && next <= '9'))) {
			while (getchar() != '\n')
				;
			return 0;
		}
		if (counter >= 20) {
			while (getchar() != '\n')
				;
			return 0;
		}
		address[counter] = next;
		next = getchar();
	}
	if (counter < 1)
		return 0;
	counter++;
	while(counter <= 19){
		address[counter] = '\0';
		counter++;
	}
	return 1;
}


int input_text(char* name, const char* what)
{
	while (!inserted_text(name, what)) {
		printf("Wrong format of %s.\nTry again\n", what);
	}
	return 1;
}

int input_address(char* address)
{
	while (!inserted_address(address)) {
		printf("Wrong format of address.\nTry again\n");
	}
	return 1;
}

void create()
{
	int num = check_numbers();
	account new;
	if (!input_text(new.name, "Name"))
		return;
	if (!input_text(new.surname, "Surname"))
		return;
	if (!input_address(new.address))
		return;
	new.number = num;
	new.balance = 0;
	new.savings = 0;
	while (1) {
		if (!input_id(new.id))
			return;
		else if (!check_id(new.id)) {
			printf("\nThis id already exists in the system.\nTry again or type "
				   "0000 to cancel account creation\n>");
		}
		else
			break;
	}
	FILE* acc_file = open_accounts("ab");
	if (confirm())
		fwrite(&new, sizeof(new), 1, acc_file);
	fclose(acc_file);
	system("clear");
}

int search_decision()
{
	printf("What do you want to search by\n1 - Acc number\n2 - Name\n3 - "
		   "Surname\n4 - Address\n5 - Id");
	int action = 0;
	while (1) {
		printf("\n> ");
		if (scanf("%d", &action)) {
			while (getchar() != '\n')
				;
			if (action < 0 || action > 5)
				continue;
			else
				break;
		}
		else {
			while (getchar() != '\n')
				;
			action = -1;
		}
	}
	return action;
}

int search_by_text(int decision, char* text)
{
	switch (decision) {
	case 2:
		if (!input_text(text, "Name"))
			return 0;
		return 1;
	case 3:
		if (!input_text(text, "Surame"))
			return 0;
		return 1;
	case 4:
		if (!input_address(text))
			return 0;
		return 1;
	}
	return 0;
}

int compare_text_search(int decision, account acc, char* text)
{
	switch (decision) {
	case 2:
		if (!strncmp(acc.name, text, 20)) {
			printf(PRINT, PARAMETERS);
			return 1;
		}
		return 0;
	case 3:
		if (!strncmp(acc.surname, text, 20)) {
			printf(PRINT, PARAMETERS);
			return 1;
		}
		return 0;
	case 4:
		if (!strncmp(acc.address, text, 20)) {
			printf(PRINT, PARAMETERS);
			return 1;
		}
		return 0;
	}
	return 0;
}

int search_by_number(int* num)
{
	printf("Number >");
	int counter = 0;
	char next = getchar();
	int digit;
	while (next != '\n') {
		counter++;
		if (next < '0' || next > '9') {
			while (getchar() != '\n')
				;
			return 0;
		}
		digit = next - '0';
		*num = *num * 10 + digit;
		next = getchar();
	}
	if (!counter)
		return 0;
	return 1;
}

int compare_number_search(account acc, int num)
{
	if (acc.number == num) {
		printf(PRINT, PARAMETERS);
		return 1;
	}
	return 0;
}

int compare_id_search(account acc, char* id)
{
	if (!strncmp(id, acc.id, 4)) {
		printf(PRINT, PARAMETERS);
		return 1;
	}
	return 0;
}

void search_acc()
{
	int decision = search_decision(), num = 0;
	FILE* file = open_accounts("rb");
	account acc;
	char text[20], id[4];
	if (decision == 2 || decision == 3 || decision == 4) {
		if (!search_by_text(decision, text)){
			fclose(file);
			return;
		}
	}
	if (decision == 5) {
		if (!input_id(id)){
			fclose(file);
			return;
		}
	}
	if (decision == 1) {
		do
			search_by_number(&num);
		while (num == 0);
	}

	int found = 0;
	while (fread(&acc, sizeof(account), 1, file) == 1) {
		if (decision == 1 && compare_number_search(acc, num))
			found++;
		if (decision >= 2 && decision <= 4 &&
			compare_text_search(decision, acc, text))
			found++;
		if (decision == 5 && compare_id_search(acc, id)) {
			found++;
		}
	}
	if (!found)
		printf("\nNo accounts found\n\n");
	fclose(file);
}

void delete_account()
{
	char id[4] = {0};

	if (!input_id(id))
		return;

	FILE* file = open_accounts("r+b");
	FILE* fp_tmp = open_tmp();
	int found = 0;
	account acc;
	while (fread(&acc, sizeof(account), 1, file) != 0) {
		if (strncmp(id, acc.id, 4) == 0) {
			printf("A record with requested name found and deleted.\n\n");
			found = 1;
		}
		else {
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
	}
	fclose(fp_tmp);
	file_exchange(found, id);
	fclose(file);
}

void list_all()
{
	system("clear");
	FILE* file = open_accounts("rb");
	account acc;
	while (fread(&acc, sizeof(account), 1, file) == 1) {
		printf(PRINT, PARAMETERS);
	}
	fclose(file);
}

void withdraw()
{
	char id[4];
	printf("Account to withdraw\n");
	if (!input_id(id))
		return;
	FILE* file = open_accounts("r+b");
	FILE* fp_tmp = open_tmp();
	account acc;
	int money = -1, found = 0;
	while (fread(&acc, sizeof(account), 1, file) != 0) {
		if (strncmp(id, acc.id, 4) == 0) {
			printf("How much money would you like to withdraw? (max %d).\n>",
				   acc.balance);
			do
				money = validity_of_trannsfer(acc.balance);
			while (money < 0);
			found = 1;
			acc.balance -= money;
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
		else {
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
	}
	fclose(fp_tmp);
	file_exchange(found, id);
	fclose(file);
}

void deposit()
{
	char id[4];
	if (!input_id(id))
		return;
	FILE* file = open_accounts("r+b");
	FILE* fp_tmp = open_tmp();
	account acc;
	int money = -1, found = 0;
	while (fread(&acc, sizeof(account), 1, file) != 0) {
		if (strncmp(id, acc.id, 4) == 0) {
			int max = INT_MAX - acc.balance;
			printf("How much money would you like to deposit?(max %d)\n>", max);
			do
				money = validity_of_trannsfer(max);
			while (money < 0);
			found = 1;
			acc.balance += money;
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
		else {
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
	}
	fclose(fp_tmp);
	file_exchange(found, id);
	fclose(file);
}

int max_transfer(int from, int to)
{
	return (from < INT_MAX - to ? from : INT_MAX - to);
}

int input_money_transfer(int max)
{
	int money = 0;
	printf("How much money would you like to transfer? (max %d).\n>", max);
	while (1) {
		money = validity_of_trannsfer(max);
		if (money < 0)
			printf("Wrong format or sum.\nTry again (max %d)\n>", max);
		else
			return money;
	}
}

void mt_file_exchange(int found, char* id_from, char* id_to)
{
	if (found < 2) {
		printf("No record(s) found with the requested id: %s or %s\n\n",
			   id_from, id_to);
	}
	if (found == 2) {
		if (confirm()) {
			remove("accounts.bin");
			rename("tmp.bin", "accounts.bin");
		}
		else
			remove("tmp.bin");
	}
}

void money_transfer()
{
	char id_from[4], id_to[4];
	printf("Provide id of account from\n>");
	if (!input_id(id_from))
		return;
	printf("Provide id of account to\n>");
	if (!input_id(id_to))
		return;
	FILE* file = open_accounts("r+b");
	FILE* fp_tmp = open_tmp();
	account acc_from, acc_to, acc;
	int money = 0, found = 0;
	while (fread(&acc, sizeof(account), 1, file) != 0) {
		if (strncmp(id_from, acc.id, 4) == 0) {
			acc_from = acc;
			found++;
		}
		else if (strncmp(id_to, acc.id, 4) == 0) {
			acc_to = acc;
			found++;
		}
		else {
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
	}
	if (found == 2) {
		int max = max_transfer(acc_from.balance, acc_to.balance);
		money = input_money_transfer(max);

		acc_from.balance -= money;
		acc_to.balance += money;
		fwrite(&acc_from, sizeof(account), 1, fp_tmp);
		fwrite(&acc_to, sizeof(account), 1, fp_tmp);
	}
	fclose(fp_tmp);
	mt_file_exchange(found, id_from, id_to);
	fclose(file);
}

int check_decision()
{
	int counter;
	while (1) {
		printf("> ");
		char decision = getchar();
		if (decision != '1' && decision != '2') {
			while (getchar() != '\n')
				;
			continue;
		}
		counter = 0;
		while (getchar() != '\n')
			counter++;
		if (!counter)
			return decision - '0';
	}
}

void savings_to_normal()
{
	char id[4];
	if (!input_id(id))
		return;
	FILE* file = open_accounts("r+b");
	FILE* fp_tmp = open_tmp();
	account acc;
	int money = 0, found = 0, decision = 0, max;
	while (fread(&acc, sizeof(account), 1, file) != 0) {
		if (strncmp(id, acc.id, 4) == 0) {
			printf("1 - To savings account\n2 - From savings account \n");
			decision = check_decision();
			if (decision == 1)
				max = max_transfer(acc.balance, acc.savings);
			else
				max = max_transfer(acc.savings, acc.balance);
			money = input_money_transfer(max);
			found = 1;
			acc.balance += (decision = 1 ? -money : money);
			acc.savings -= (decision = 1 ? -money : money);
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
		else {
			fwrite(&acc, sizeof(account), 1, fp_tmp);
		}
	}
	fclose(fp_tmp);
	file_exchange(found, id);
	fclose(file);
}

int decision()
{
	int action = -1;
	printf("1 - create\n2 - delete\n3 - list all\n4 - deposit\n5 - "
		   "withdraw\n6 - money transfer\n7 - transfer from/to savings "
		   "account\n8 - search\n9 - exit");
	while (1) {
		printf("\nAction> ");
		if (scanf("%d", &action)) {
			while (getchar() != '\n')
				;
			if (action <= 0 || action > 9)
				continue;
			else
				break;
		}
		else {
			while (getchar() != '\n')
				;
			action = -1;
		}
	}
	return action;
}

void initialise()
{
	FILE* file;
	if (fopen("tmp.bin", "rb")){
		printf("Last sesion was closed incorectly");
	}
	if (!(file = fopen("accounts.bin", "rb")))
		file = fopen("accounts.bin", "wb");
	fclose(file);
}

int main(int argc, char* argv[])
{
	initialise();
	int action = -1;
	while (action != 9) {
		action = decision();
		switch (action) {
/*		case 0:
			reset();
			break;*/
		case 1:
			create();
			break;
		case 2:
			delete_account();
			break;
		case 3:
			list_all();
			break;
		case 4:
			deposit();
			break;
		case 5:
			withdraw();
			break;
		case 6:
			money_transfer();
			break;
		case 7:
			savings_to_normal();
			break;
		case 8:
			search_acc();
			break;
		}
	}
	return 0;
}
