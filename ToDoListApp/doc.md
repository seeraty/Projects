# Documentation on code

## Import Explanation

### Java IO

- `import java.io.*`: Imports Java's input and output classes, such as FileReader, FileWriter, and IOException, to handle reading from and writing to files.

### Java NIO

- `import java.nio.file.*`: Provides methods to work with files and directories, such as reading, writing, and deleting. It also includes classes like Path and Paths for file path manipulations, and StandardOpenOption for defining standard open options for file operations.

### Java Time
The application uses Java's built-in LocalDate, DateTimeFormatter, and other date and time-related classes for date manipulation and formatting.

- `import java.time.*`: Provides classes that represent dates, times, and durations (periods and durations), including LocalDate and Month.
- `import java.time.format.*`: Provides classes for printing and parsing dates and times, including DateTimeFormatter.
- `import java.time.temporal.*`: Provides classes for date and time calculations, including TemporalAdjusters and WeekFields.

### Java Utility

- `import java.util.*`: Imports various Java utility classes and interfaces, such as HashMap, List, and Scanner.
- `import java.util.stream.Stream`: Represents a sequence of elements and supports various operations, such as filtering, mapping, and iteration.

## Main code

### Class Variables

- `FILE_NAME`: The name of the file that stores the tasks.
- `DATE_FORMATTER`: The date format used in the application.
- `today`: Stores the current date.
- `tasks`: A HashMap that stores the tasks, with dates as keys and lists of tasks as values.
- `scanner`: A Scanner object for reading user input.

### Main Method

- `main(String[] args)`: The main method of the ToDoList application that handles user input and invokes corresponding methods.

### Main Functionalities

- `addTask(String task, String date)`: Adds a new task to the list and writes it to the file.
- `editTask(String oldTask, String newTask, String date)`: Edits an existing task and updates the file.
- `removeTask(String task, String date)`: Removes a task from the list and updates the file.
- `viewTasks(String option)`: Displays tasks for the specified time period (daily, weekly, or monthly).


### Helper Methods

- `getInput(String message)`: Gets user input and returns it as a String.
- `runIfTasksExistInSpecifiedTimePeriod(LocalDate startDate, LocalDate endDate)`: Checks if tasks exist in the specified time period and runs code accordingly.
- `formatTask(String date, String task)`: Formats data to be added to the file.
- `checkIfTaskExists(String date, String taskToFind)`: Checks if a task already exists in the tasks HashMap for a specified date.
- `getFirstDayOfWeek(String week)`: Gets the first day of the week for the specified week number.
- `getFirstDayOfMonth(String month)`: Gets the first day of the specified month.
- `formatDate(String inputDate)`: Formats a given date string to the format "dd-MM-yyyy".
- `reverseDate(String date)`: Reverses the order of a date string.
- `loadDataFromFile()`: Reads the tasks from the file and populates the tasks HashMap.
- `welcomeMessage()`: Displays a welcome message to the user.
- `runToDoList()`: Runs the main logic of the ToDoList application.
- `navigateTimePeriods(LocalDate dateOfChoice, int periodLength, String periodType)`: Navigates through different time periods (day, week, month) and displays tasks accordingly.
- `displayTasks(String date)`: Displays tasks for a given date.

### Exceptions
- `IOException`: Thrown by methods that read from or write to the file, such as `addTask`, `editTask`, `removeTask`, `loadDataFromFile`, and `runToDoList`.
- `NumberFormatException`: Thrown by the `getFirstDayOfWeek` method when the week number is not a valid integer.
- `DateTimeException`: Thrown by the `getFirstDayOfWeek` method when the week number is out of range.
- `IllegalArgumentException`: Thrown by the `getFirstDayOfMonth` method when the month name is not a valid Month enum name.


