#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task {
    char *name;
    int priority;
    struct task *next;
} task;

typedef struct list {
    task *head;
    int size;
} list;

task *create_task(char *name, int priority) {
    task *new_task = malloc(sizeof(task));
    new_task->name = strdup(name);
    new_task->priority = priority;
    new_task->next = NULL;
    return new_task;
}

list *create_list() {
    list *new_list = malloc(sizeof(list));
    new_list->head = NULL;
    new_list->size = 0;
    return new_list;
}

void add_task(list *l, task *t) {
    if (l == NULL || t == NULL) return;

    if (l->head == NULL || t->priority > l->head->priority) {
        t->next = l->head;
        l->head = t;
    } else {
        task *current = l->head;
        while (current->next != NULL && t->priority <= current->next->priority) {
            current = current->next;
        }
        t->next = current->next;
        current->next = t;
    }
    l->size++;
}

void remove_task(list *l, char *name) {
    if (l == NULL || name == NULL || l->head == NULL) return;

    if (strcmp(l->head->name, name) == 0) {
        task *temp = l->head;
        l->head = l->head->next;
        free(temp->name);
        free(temp);
        l->size--;
    } else {
        task *current = l->head;
        while (current->next != NULL && strcmp(current->next->name, name) != 0) {
            current = current->next;
        }
        if (current->next != NULL) {
            task *temp = current->next;
            current->next = current->next->next;
            free(temp->name);
            free(temp);
            l->size--;
        }
    }
}

void delete_list(list *l) {
    if (l == NULL) return;

    task *current = l->head;
    while (current != NULL) {
        task *temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
    free(l);
}

void print_list(list *l) {
    if (l == NULL) return;

    printf("To-do list:\n");
    task *current = l->head;
    while (current != NULL) {
        printf("- %s (priority: %d)\n", current->name, current->priority);
        current = current->next;
    }
    printf("\n");
}

void save_tasks(list *l) {
    if (l == NULL) return;

    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to save tasks to file.\n");
        return;
    }

    task *current = l->head;
    while (current != NULL) {
        fprintf(file, "%s %d\n", current->name, current->priority);
        current = current->next;
    }

    fclose(file);
}

void load_tasks(list *l) {
    if (l == NULL) return;

    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        printf("No existing tasks found.\n");
        return;
    }

    char name[100];
    int priority;
    while (fscanf(file, "%99s %d", name, &priority) == 2) {
        task *t = create_task(name, priority);
        add_task(l, t);
    }

    fclose(file);
}

void display_help() {
    printf("\nCommands:\n");
    printf("  1. 'a': Add a task\n");
    printf("  2. 'r': Remove a task\n");
    printf("  3. 'p': Print the list\n");
    printf("  4. 'x': Exit the program\n");
    printf("  5. 'h': Display help\n");
}

int process_command(char command, list *l) {
    switch (command) {
        case 'a':
            {
                char name[100];
                int priority;
                printf("Enter the name of the task: ");
                scanf("%99s", name);
                printf("Enter the priority of the task (1-5): ");
                scanf("%d", &priority);
                task *t = create_task(name, priority);
                add_task(l, t);
                printf("Task added successfully!\n\n");
            }
            break;
        case 'r':
            {
                char name[100];
                printf("Enter the name of the task to be removed: ");
                scanf("%99s", name);
                remove_task(l, name);
                printf("Task removed successfully!\n\n");
            }
            break;
        case 'p':
            print_list(l);
            break;
        case 'x':
            printf("Saving tasks and exiting the program...\n");
            save_tasks(l);
            return 1;
        case 'h':
            display_help();
            break;
        default:
            printf("Invalid command! Type 'h' for help.\n");
    }
    return 0;
}

int main() {
    list *l = create_list();
    load_tasks(l);
    int exit_program = 0;

    do {
        char input[100];
        printf("Enter a command ('h' for help): ");
        scanf("%99s", input);

        char command = input[0];
        while (getchar() != '\n') continue;

        exit_program = process_command(command, l);
    } while (!exit_program);

    delete_list(l);
    return 0;
}
