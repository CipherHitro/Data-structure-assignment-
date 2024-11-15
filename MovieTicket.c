#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure for a linked list (representing a seat)
typedef struct SeatNode {
    int seatNumber;
    int isBooked;
    struct SeatNode* next;
} SeatNode;

// Queue structure to manage booking requests
typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
} Queue;

// Function to create a queue of given capacity
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

// Check if the queue is full
int isFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

// Check if the queue is empty
int isEmpty(Queue* queue) {
    return (queue->size == 0);
}

// Enqueue a booking request
void enqueue(Queue* queue, int seatNumber) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = seatNumber;
    queue->size = queue->size + 1;
}

// Dequeue a booking request
int dequeue(Queue* queue) {
    if (isEmpty(queue)) return -1;
    int seatNumber = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return seatNumber;
}

// Initialize seats for a row using linked list
SeatNode* initializeRow(int rowNumber, int cols) {
    SeatNode* head = NULL;
    SeatNode* current = NULL;
    for (int i = 1; i <= cols; i++) {
        SeatNode* newSeat = (SeatNode*)malloc(sizeof(SeatNode));
        newSeat->seatNumber = (rowNumber - 1) * cols + i;
        newSeat->isBooked = 0;
        newSeat->next = NULL;
        if (head == NULL) {
            head = newSeat;
            current = head;
        } else {
            current->next = newSeat;
            current = newSeat;
        }
    }
    return head;
}

// Display seat availability
void displaySeats(SeatNode* rows[], int row_count) {
    printf("\nSeat Availability:\n");
    for (int i = 0; i < row_count; i++) {
        SeatNode* current = rows[i];
        printf("Row %d: ", i + 1);
        while (current) {
            printf("%d[%s] ", current->seatNumber, current->isBooked ? "Booked" : "Free");
            current = current->next;
        }
        printf("\n");
    }
}

// Book a seat if available
void bookSeat(SeatNode* rows[], int seatNumber, int cols) {
    int row = (seatNumber - 1) / cols;
    int col = (seatNumber - 1) % cols;
    SeatNode* current = rows[row];
    for (int i = 0; i < col; i++) {
        current = current->next;
    }
    if (current && !current->isBooked) {
        current->isBooked = 1;
        printf("Seat %d successfully booked.\n", seatNumber);
    } else {
        printf("Seat %d is already booked or invalid.\n", seatNumber);
    }
}

// Cancel a booking if the seat is currently booked
void cancelBooking(SeatNode* rows[], int seatNumber, int cols) {
    int row = (seatNumber - 1) / cols;
    int col = (seatNumber - 1) % cols;
    SeatNode* current = rows[row];
    for (int i = 0; i < col; i++) {
        current = current->next;
    }
    if (current && current->isBooked) {
        current->isBooked = 0;
        printf("Booking for seat %d has been canceled.\n", seatNumber);
    } else {
        printf("Seat %d is not currently booked or invalid.\n", seatNumber);
    }
}

// Show statistics for total, booked, and available seats
void showStatistics(SeatNode* rows[], int row_count) {
    int totalSeats = 0, bookedSeats = 0, availableSeats = 0;
    for (int i = 0; i < row_count; i++) {
        SeatNode* current = rows[i];
        while (current) {
            totalSeats++;
            if (current->isBooked) bookedSeats++;
            current = current->next;
        }
    }
    availableSeats = totalSeats - bookedSeats;
    printf("\nStatistics:\n");
    printf("Total Seats: %d\n", totalSeats);
    printf("Booked Seats: %d\n", bookedSeats);
    printf("Available Seats: %d\n", availableSeats);
}

int main() {
    int movieChoice;
    const int rows = 5;   // Fixed 5 rows
    const int cols = 10;  // Fixed 10 seats per row
    char* movies[] = {
        "1. Avengers: Endgame",
        "2. Inception",
        "3. Interstellar",
        "4. The Dark Knight"
    };

    // Display movie list and get user choice
    printf("Available Movies:\n");
    for (int i = 0; i < 4; i++) {
        printf("%s\n", movies[i]);
    }
    printf("Select a movie to book tickets (1-4): ");
    scanf("%d", &movieChoice);

    if (movieChoice < 1 || movieChoice > 4) {
        printf("Invalid movie choice. Exiting system.\n");
        return 0;
    }
    printf("You selected: %s\n", movies[movieChoice - 1]);

    // Initialize queue to manage booking requests
    Queue* bookingQueue = createQueue(50);

    // Initialize theater seats
    SeatNode* rowList[rows];
    for (int i = 0; i < rows; i++) {
        rowList[i] = initializeRow(i + 1, cols);
    }

    int choice, seatNumber;
    while (1) {
        printf("\n--- Online Movie Ticket Booking System ---\n");
        printf("1. Display Seat Availability\n");
        printf("2. Add Booking Request\n");
        printf("3. Process Booking Request\n");
        printf("4. Cancel Booking\n");
        printf("5. Show Statistics\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displaySeats(rowList, rows);
                break;

            case 2:
                printf("Enter seat number to request booking: ");
                scanf("%d", &seatNumber);
                if (seatNumber < 1 || seatNumber > 50) {
                    printf("Invalid seat number. Please select a seat between 1 and 50.\n");
                } else {
                    enqueue(bookingQueue, seatNumber);
                    printf("Booking request for seat %d added to queue.\n", seatNumber);
                }
                break;

            case 3:
                if (!isEmpty(bookingQueue)) {
                    seatNumber = dequeue(bookingQueue);
                    bookSeat(rowList, seatNumber, cols);
                } else {
                    printf("No booking requests in the queue.\n");
                }
                break;

            case 4:
                printf("Enter seat number to cancel booking: ");
                scanf("%d", &seatNumber);
                if (seatNumber < 1 || seatNumber > 50) {
                    printf("Invalid seat number. Please select a seat between 1 and 50.\n");
                } else {
                    cancelBooking(rowList, seatNumber, cols);
                }
                break;

            case 5:
                showStatistics(rowList, rows);
                break;

            case 6:
                printf("Exiting system. Thank you for using the movie ticket booking system.\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
