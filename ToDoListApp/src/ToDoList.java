import java.io.*;
import java.nio.file.*;
import java.time.*;
import java.time.format.*;
import java.time.temporal.*;
import java.util.*;
import java.util.stream.Stream;

/**
 * Main class. Creates, stores, edits and deletes tasks. It also allows users to
 * communicate with the program. The user can also view tasks in a specific
 * time period (eg date, week, month).
 *
 * @author Seerat Yasir
 * @author Ava Vatanchi
 * @version 03/05/2023
 */
public class ToDoList {
    private static final String FILE_NAME = "StoredToDoLists.txt";
    private static final Map<String, List<String>> tasks = new LinkedHashMap<>();
    private static final Scanner scanner = new Scanner(System.in);
    private static final DateTimeFormatter DATE_FORMATTER = DateTimeFormatter.ofPattern("dd-MM-yyyy");
    private static final LocalDate today = LocalDate.now();

    // Main method
    public static void main(String[] args) throws Exception {
        loadDataFromFile();
        welcomeMessage();
        runToDoList();
    }

    /**
     * Loads tasks from file into map "tasks". Renews map each time the method is
     * called by clearing its previous data.
     */
    private static void loadDataFromFile() throws IOException {
        tasks.clear();
        Path filePath = Paths.get(FILE_NAME);

        if (!Files.exists(filePath)) {
            Files.createFile(filePath); // If file does not exist, create one
        } else {
            try {
                List<String> lines = Files.readAllLines(filePath);

                for (String line : lines) {
                    String[] parts = line.split(" ", 2); // Split string into date and task
                    if (parts.length >= 2) {
                        String taskDate = formatDate(parts[0]);
                        String taskName = parts[1];

                        if (taskDate != null) { // Put date and task as key-value pair
                            tasks.computeIfAbsent(taskDate, k -> new ArrayList<>()).add(taskName);
                        } else {
                            // In case somehow data written in file is in a wrong format
                            System.err.println("Invalid data format in file: " + line);
                        }
                    } else {
                        System.err.println("Invalid data format in file: " + line);
                    }
                }
            } catch (IOException e) {
                System.err.println("Error reading data from file.");
                e.printStackTrace();
            }
        }
    }

    /**
     * Welcome message which appears when the user first run the program.
     * Welcomes the user and displays current week's tasks if any exist.
     */
    private static void welcomeMessage() {
        System.out.println("\nWelcome back!\n");

        // Get current week number, start and end date of week
        int weekNumber = today.get(WeekFields.ISO.weekOfWeekBasedYear());
        LocalDate startOfWeek = today.minusDays(today.getDayOfWeek().getValue() - 1);
        LocalDate endOfWeek = startOfWeek.plusDays(6);

        if (runIfTasksExistInSpecifiedTimePeriod(startOfWeek, endOfWeek)) {
            System.out.println("To Do list for current week (week " + weekNumber + "):");
            for (int i = 0; i < 7; i++) {
                LocalDate date = startOfWeek.plusDays(i);
                displayTasks(date.format(DATE_FORMATTER));
            }
        } else {
            System.out.println("You have no tasks this week! You may rest or add some tasks :)");
        }
    }

    /**
     * Main method which user interact with. User is prompted to choose an option and
     * relevant case is executed. Renews the map each time a task is added, edited or removed.
     *
     * @throws IOException if methods add, edit and remove fail.
     */
    private static void runToDoList() throws IOException {
        boolean exit = false;
        while (!exit) {
            String command = getInput(
                    "Please choose an option: View, Add, Remove, Edit, Exit. For more information of the options and shortcuts, type \"help\".");
            switch (command.toLowerCase()) {
                case "h","help" -> System.out.println(
                        """
                                \nA guide to all options.

                                View/v: Displays To-Do Lists of chosen time period and navigate to previous or next time period by typing 'next'/'n', 'previous'/'prev'/'p' or 'back'/'b' when prompted.\s
                                Time periods to choose from when prompted: 'day'/'d', 'week'/'w', 'month'/'m'. Option 'all'/'a' is for viewing all To-Do lists stored.\s
                                Note that empty To-Do lists are not displayed. For option 'day', dates can be formatted in following ways: dd/MM/yyyy, dd.MM.yyyy, dd-MM-yyyy and ddMMyyyy. Option 'today'/'t' displays current day's To Do list. For 'week', a week 1-53 must be chosen, or type 'current' to display current week's tasks. For 'month', input can be formatted as a three letter abbreviation, number (with or without a leading 0) or full name of the month. Option 'current' is available for 'month' too.

                                Add/a: Adds a task to a To-Do List. Write title/description of the task and deadline when prompted. The deadline must be written in correct format (see above).

                                Remove/rm/r/del: Removes specified task. Write title/description and date of the task to remove. Make sure that the spelling is correct and use the 'view' option to make sure you chose the right task/ make no typos. Note that there is no undo option! The task is deleted permanently\s

                                Edit/e: Edits the specified task's title/description and/or deadline. Input task-title and deadline of the task you want to edit as well as the new title and/or deadline. If you do not wish to change title or deadline, type 'same' when prompted for new title/date.

                                Exit/esc/q: If viewing mode is open, this command exits the mode and goes back to the starting prompt (i.e. 'Please choose an option: View, Add, Remove, Edit, Exit'). If command is input in the starting prompt, the program is exited. All tasks added and edited are saved and reopened when starting the program again.""");
                case "v","view" -> viewTasks(); // see javadoc comment for viewTasks() for information
                case "a","add" -> {
                    String task = getInput("Write a short description or title to your task.");
                    String date = getInput("Write the deadline in correct format. See 'help' for details.");
                    addTask(date, task);
                }
                case "rm","del","r","remove" -> {
                    String taskToRemove = getInput("Write the title of the task you wish to remove.");
                    String dateOfTaskToRemove = getInput("Write the task's deadline in correct format. See 'help' for details.");
                    removeTask(taskToRemove, dateOfTaskToRemove);
                }
                case "e","edit" -> {
                    String originalTaskTitle = getInput("Write the title of the task you wish to edit.");
                    String originalDate = getInput("Write the task's original deadline in correct format. See 'help' for details.");
                    String newTaskTitle = getInput(
                            "Write new title to the task. Write 'same' if you do not wish to change the title.");
                    String newDate = getInput(
                            "Write new deadline to the task in correct format. Write 'same' if you do not wish to change the deadline.");
                    editTask(originalTaskTitle, originalDate, newTaskTitle, newDate);
                }
                case "esc","q","exit" -> {
                    System.out.println("\nGoodbye!");
                    scanner.close();
                    exit = true;
                }
                default -> System.out.println("\nInvalid command.");
            }
        }
    }

    /**
     * Continuation of runToDoList() for option view. Displays and lets user
     * navigate user inputs (i.e. tasks for a day, week, month, all, or a specific date).
     * This is done by the 'navigateTimePeriods' method.
     */
    private static void viewTasks() {
        boolean exit = false;
        while (!exit) {
            String input = getInput("Enter 'day', 'week', 'month', 'all' or 'exit':").toLowerCase();

            switch (input) { // Handle user input based on different cases.
                case "d","day" -> {
                    String dateInput = getInput(
                        "Enter date to view tasks, type 'today' to see tasks for the day or press enter to exit viewing mode:").toLowerCase();
                    if (!dateInput.isBlank()) { // exits if user gives no input
                        if (dateInput.equals("today") || dateInput.equals("t")) {
                            navigateTimePeriods(today, 1, "day");
                        } else {
                            String formattedDateInput = formatDate(dateInput); // Formats input date. Returns null + error
                            if (formattedDateInput != null) {                  // message for incorrect input.
                                LocalDate stringInput = LocalDate.parse(formattedDateInput, DATE_FORMATTER);
                                navigateTimePeriods(stringInput, 1, "day");
                            }
                        }
                    }
                }
                case "w","week" -> {
                    String weekInput = getInput(
                        "Enter week number (1-53) to view tasks, type 'current' to see tasks for this week or press enter to exit viewing mode:");
                    if (!weekInput.isBlank()) {
                        LocalDate firstDayOfChosenWeek = getFirstDayOfWeek(weekInput); // See its javadoc comment for return types
                        if (firstDayOfChosenWeek != null) {
                            navigateTimePeriods(firstDayOfChosenWeek, 7, "week");
                        }
                    }
                }
                case "m","month" -> {
                    String monthInput = getInput(
                        "Enter month to view tasks (input 3 letter abbreviation, full name, or number of month), type 'current' to see tasks for this month or press enter to exit viewing mode:");
                    if (!monthInput.isBlank()) {
                        LocalDate firstDayOfChosenMonth = getFirstDayOfMonth(monthInput); // See its javadoc comment for return types
                        if (firstDayOfChosenMonth != null) {
                            navigateTimePeriods(firstDayOfChosenMonth, 0, "month");
                        } // period length for month will be fixed in navigateTimePeriods itself.
                    }
                }
                case "a","all" -> {
                    System.out.println("All To Do lists in chronological order:");
                    List<String> dateList = new ArrayList<>();
                    // Convert all dates into format yyyy-MM-dd and add into dateList
                    for (String date : tasks.keySet()) {
                        String formattedDate = reverseDate(date);
                        dateList.add(formattedDate);
                    }
                    Collections.sort(dateList); // Sort sortList (now the dates will be in chronological order)
                    // Iterate over sorted dateList, reverse date again and display tasks for each date
                    for (String date : dateList) {
                        String originalFormatDate = reverseDate(date);
                        displayTasks(originalFormatDate);
                    }
                }
                case "esc", "q", "exit" -> exit = true; // Return to runToDoList()
                default -> System.out.println("\nInvalid command. Try again.");
            }
        }
    }


    /**
     * Navigates through time periods and displays tasks.
     *
     * @param dateOfChoice date to display or starting date to display week or month for.
     *                     Critical to get this date correct for the method to work.
     * @param periodLength length of time period to display tasks for
     * @param periodType   the type of period to navigate (day, week or month).
     */
    private static void navigateTimePeriods(LocalDate dateOfChoice, int periodLength, String periodType) {
        while (true) {
            LocalDate startDate = dateOfChoice;
            LocalDate endDate; // Choose endDate by adding appropriate amount of days to startDate
            if (periodType.equals("month")) { // Needed because length of months vary
                endDate = startDate.plusDays(startDate.lengthOfMonth() - 1);
            } else {
                endDate = startDate.plusDays(periodLength - 1);
            }

            // If tasks exist between startDate to endDate, loop through day/days and
            // display To Do lists
            String message;
            switch (periodType) {
                case "week" -> message = " week " + startDate.get(WeekFields.ISO.weekOfWeekBasedYear());
                case "month" -> message = startDate.format(DateTimeFormatter.ofPattern(" MMMM yyyy"));
                default -> message = "";
            }
            if (runIfTasksExistInSpecifiedTimePeriod(startDate, endDate)) {
                System.out.println("\nTo Do list for" + message + ":");
                for (LocalDate date = startDate; !date.isAfter(endDate); date = date.plusDays(1)) {
                    displayTasks(date.format(DATE_FORMATTER));
                }
            } else {
                if (periodType.trim().equalsIgnoreCase("day")) {
                    System.out.println("\nThere are no tasks for " + startDate.format(DateTimeFormatter.ofPattern("d MMMM u")) + ".");
                } else {
                    System.out.println("\nThere are no tasks for" + message + ".");
                }
            }

            // Display navigation options and get user input
            String input = getInput("Options: 'next' to view next " + periodType + ", 'previous' to view previous "
                    + periodType + " and 'back' to return.");

            // Navigate by increasing/decreasing a day/week/month and re-running method with new dateOfChoice
            // If input is invalid, loop doesn't exit so the user gets chances to write the right input
            boolean validInput = false;
            while (!validInput) {
                switch (input.toLowerCase()) {
                    case "n", "next" -> {
                        dateOfChoice = switch (periodType.toLowerCase()) {
                            case "day" -> dateOfChoice.plusDays(1);
                            case "week" -> dateOfChoice.plusWeeks(1);
                            case "month" -> dateOfChoice.plusMonths(1).withDayOfMonth(1);
                            default -> throw new IllegalArgumentException("\nUnsupported period type: " + periodType);
                        };
                        validInput = true;
                    }
                    case "p", "prev", "previous" -> {
                        dateOfChoice = switch (periodType.toLowerCase()) {
                            case "day" -> dateOfChoice.minusDays(1);
                            case "week" -> dateOfChoice.minusWeeks(1);
                            case "month" -> dateOfChoice.minusMonths(1).withDayOfMonth(1);
                            default -> throw new IllegalArgumentException("\nUnsupported period type: " + periodType);
                        };
                        validInput = true;
                    }
                    case "b", "back" -> {
                        return;
                    }
                    default -> {
                        System.out.println("\nInvalid input. Try again.");
                        input = getInput("Options: 'next' to view next " + periodType + ", 'previous' to view previous "
                                + periodType + " and 'exit'.");
                    }
                }
            }
        }
    }

    /**
     * Displays tasks for a given date. Doesn't display days without tasks.
     *
     * @param date the date as a string in the format "dd-MM-yyyy"
     */
    private static void displayTasks(String date) {
        List<String> taskList = tasks.get(date); // Get all tasks for a date
        // Make sure list + tasks exist in date mentioned
        if (taskList != null && !taskList.isEmpty()) {
            // Display date in the required format
            System.out.println("\n" +
                    LocalDate.parse(date, DATE_FORMATTER).format(DateTimeFormatter.ofPattern("EEEE d MMM, u", Locale.ENGLISH)));
            for (int i = 0; i < taskList.size(); i++) {
                // Display task index along with the task.
                System.out.println((i + 1) + ". " + taskList.get(i));
            }
        }
    }

    /**
     * Writes given task and date into the bottom of file StoredToDoLists.txt.
     * If same task exists the date given, task is not added.
     *
     * @param task is a String description of the user's task
     * @param date is the deadline date (String) the user sets their task to
     */
    private static void addTask(String date, String task) throws IOException {
        String formattedDate = formatDate(date);
        if (formattedDate == null) {
            return; // error message given from formatDate()
        }
        if (checkIfTaskExists(formattedDate, task)) {
            System.out.println(
                    "The task you are trying to add already exists. You cannot add duplicate tasks on the same date!");
            return;
        }

        // Write task + date to file, update HashMap and give user feedback.
        Path filePath = Paths.get(FILE_NAME);
        Files.write(filePath, formatTask(formattedDate, task).getBytes(), StandardOpenOption.APPEND);
        loadDataFromFile();
        System.out.println("\nYour task '" + task + "' was added to " + formattedDate + " successfully!");
    }

    /**
     * Edits the task according to the user's changes. To edit, a new file is
     * created with the change and replaces the old one (which is later deleted).
     * If same task exists the date given, task is not added.
     * Keyword "same" keeps the old task title or date.
     *
     * @param oldTask title/description to be changed
     * @param oldDate deadline to change
     * @param newTask new title/description to be changed to
     * @param newDate new deadline to be changed to
     */
    private static void editTask(String oldTask, String oldDate, String newTask, String newDate) throws IOException {
        String formattedOldDate = formatDate(oldDate);
        String formattedNewDate;
        if (newDate.equalsIgnoreCase("same")) {
            formattedNewDate = formattedOldDate;
        } else {
            formattedNewDate = formatDate(newDate);
        }

        // "error" handling
        if (formattedOldDate == null || formattedNewDate == null) {
            return; // error message given from formatDate()
        }
        if (!checkIfTaskExists(formattedOldDate, oldTask)) { // if old task does not exist
            System.out.println("\nTask does not exist! Check your spelling or view the task to find it and try again.");
            return;
        }
        if (checkIfTaskExists(formattedNewDate, newTask)) { // if edited task already exists
            System.out.println("\nYour edit makes the task a duplicate of an already existing task the same day. " +
                    "You cannot add duplicate tasks on the same date!");
            return;
        }

        // Code-block to edit line in file
        String originalTask = formatTask(formattedOldDate, oldTask).toLowerCase();
        String editedTask;  // format edited task taking keyword "same" into consideration.
        if (newTask.equalsIgnoreCase("same") && newDate.equalsIgnoreCase("same")) {
            System.out.println("You have not changed anything! Try again.");
            return;
        } else if (newTask.equalsIgnoreCase("same")) {
            editedTask = formatTask(formattedNewDate, oldTask);
        } else if (newDate.equalsIgnoreCase("same")) {
            editedTask = formatTask(formattedOldDate, newTask);
        } else {
            editedTask = formatTask(formattedNewDate, newTask);
        }
        // Get and create file paths. Needed for Files and FileWriter methods.
        Path filePath = Paths.get(FILE_NAME);
        Path newFilePath = Paths.get(FILE_NAME + ".tmp");
        // Loop through lines in our original file and copy contents into newFile.
        // The task to be edited will be edited and added into file.
        Stream<String> lines = Files.lines(filePath);
        try (PrintWriter pw = new PrintWriter(new FileWriter(newFilePath.toFile()))) {
            lines.forEach(line -> {
                if (line.equalsIgnoreCase(originalTask.trim())) {
                    pw.print(editedTask);
                } else {
                    pw.println(line);
                }
            });
        }
        lines.close();
        // Delete old file and replace its place with the new file. Update HashMap + give user feedback.
        Files.delete(filePath);
        Files.move(newFilePath, filePath);
        loadDataFromFile();

        // Display String newTask (+ date) to user
        String newTaskToPrint;
        if (newTask.equalsIgnoreCase("same")) {
            newTaskToPrint = oldTask;
        } else {
            newTaskToPrint = newTask;
        }
        System.out.println("\nThe task " + oldTask + " on " + formattedOldDate + " has been changed to the following:\n"
                + "Task title: " + newTaskToPrint + ". Deadline: " + formattedNewDate + ".");
    }

    /**
     * Removes the task on date input if it exists.
     *
     * @param task is a String description of the user's task
     * @param date is the deadline date (String) the user sets their task to
     */
    private static void removeTask(String task, String date) throws IOException {
        String formattedDate = formatDate(date);
        if (formattedDate == null) {
            return; // error message given from formatDate()
        }
        if (!checkIfTaskExists(formattedDate, task)) { // if task does not exist
            System.out.println("\nTask does not exist! Check your spelling or view the task to find it and try again.");
            return;
        }
        String taskToRemove = formatTask(formattedDate, task);

        // Get and create file paths. Needed for Files and FileWriter methods.
        Path filePath = Paths.get(FILE_NAME);
        Path newFilePath = Paths.get(FILE_NAME + ".tmp");
        // Copy contents from the original file into newFile, excluding the task to be removed.
        Stream<String> lines = Files.lines(filePath);
        try (PrintWriter pw = new PrintWriter(new FileWriter(newFilePath.toFile()))) {
            lines.forEach(line -> {
                if (!line.equalsIgnoreCase(taskToRemove.trim())) {
                    pw.println(line);
                }
            });
        }
        lines.close();
        // Delete old file and replace its place with the new file. Update HashMap and give user feedback.
        Files.delete(filePath);
        Files.move(newFilePath, filePath);
        loadDataFromFile();
        System.out.println("\nTask " + task + " on " + formattedDate + " has been successfully removed.");
    }

    /*
     * Helper methods
     */

    /**
     * Getter for user input. Prints a message from param and returns user input.
     * Note: Return is trimmed
     *
     * @param message String to communicate/instruct to the user
     * @return a String of user input
     */
    private static String getInput(String message) {
        System.out.println("\n" + message);
        return scanner.nextLine().trim();
    }

    /**
     * Helps methods to run some code if tasks exist in the specified time period.
     * Note that date is only created to add tasks, so containsKey() thereby works.
     *
     * @param startDate date to start searching from
     * @param endDate   last date to search
     * @return true if a task was found in the time period. False if not.
     */
    private static boolean runIfTasksExistInSpecifiedTimePeriod(LocalDate startDate, LocalDate endDate) {
        LocalDate currentDate = startDate;
        while (!currentDate.isAfter(endDate)) {
            if (tasks.containsKey(currentDate.format(DATE_FORMATTER))) {
                return true;
            }
            currentDate = currentDate.plusDays(1);
        }
        return false;
    }

    /**
     * Formats data to be added to the file. Used by 'addTask' and 'editTask'
     *
     * @param task becomes trimmed
     * @param date should be in correct format (dd-MM-yyyy)
     * @return "[date] [task]" and newline
     */
    private static String formatTask(String date, String task) {
        return date.trim() + " " + task.trim() + "\n";
    }

    /**
     * Checks map for a specified task for a date and returns true if it does.
     * This method prevents writing duplicate tasks.
     * Note that date must be correctly formatted.
     *
     * @param taskToFind String to search
     * @return true if duplicate to task exists, false otherwise
     */
    private static boolean checkIfTaskExists(String date, String taskToFind) {
        List<String> tasksForDate = tasks.get(date); // Get a lists of tasks for day
        // Search list for taskToFind and return true if found (ignoring case), false otherwise.
        return tasksForDate != null && tasksForDate.stream().anyMatch(task -> task.equalsIgnoreCase(taskToFind));
    }

    /**
     * Method to get first day of week inputted. Returns current week for keyword
     * "current", correct week for numeric input and null + error messages for
     * invalid inputs. Note: Some years contain 53 weeks which we included.
     *
     * @param week The week number (1-53).
     * @return the first day of the week of week inputted and null if week is wrong
     */
    private static LocalDate getFirstDayOfWeek(String week) {
        if (week.equalsIgnoreCase("current") || week.equalsIgnoreCase("c")) {
            LocalDate currentDate = today.with(TemporalAdjusters.previousOrSame(java.time.DayOfWeek.MONDAY));
            return LocalDate.parse(currentDate.format(DATE_FORMATTER), DATE_FORMATTER); // explained below
        } else {
            try {
                int weekNumber = Integer.parseInt(week);
                if (weekNumber < 1 || weekNumber > 53) {
                    System.out.println("\nInvalid week number. Please choose a week number between 1 and 53.");
                    return null;
                } else {
                    // get the first day of given week, format as dd-MM-yyyy and return as LocalDate
                    WeekFields weekFields = WeekFields.of(Locale.getDefault());
                    int year = today.getYear();
                    LocalDate firstDayOfWeek = LocalDate.now()
                            .withYear(year) // current year
                            .with(weekFields.weekBasedYear(), year)
                            .with(weekFields.weekOfWeekBasedYear(), weekNumber) // current week
                            .with(weekFields.dayOfWeek(), 1); // monday of current week
                    String formattedDate = firstDayOfWeek.format(DATE_FORMATTER); // format date dd-MM-yyyy
                    return LocalDate.parse(formattedDate, DATE_FORMATTER); // convert into a LocalDate object
                }
            } catch (NumberFormatException e) {
                System.out.println("\nInvalid week number.");
                return null;
            } catch (DateTimeException e) {
                System.out.println(
                        "\nThis year does not consist of week number 53. Please choose a valid week number between 1 and 52.");
                return null;
            }
        }
    }

    /**
     * Method to get first day of month inputted. Returns current month for keyword
     * "current", correct week for numeric/alphabetic input and null + error
     * messages for invalid inputs.
     *
     * @param month String month as a 3-letter abbreviation, full name or number.
     * @return first day of chosen month. Null + error messages for invalid input.
     */
    private static LocalDate getFirstDayOfMonth(String month) {
        int monthNumber;
        if (month.equalsIgnoreCase("current") || month.equalsIgnoreCase("c")) {
            monthNumber = today.getMonth().getValue();
        } else {
            try {
                monthNumber = Integer.parseInt(month); // Parse input month
            } catch (NumberFormatException e) {
                if (month.length() == 3) { // i.e. 3 letter abbreviation
                    monthNumber = -1;
                    String monthUpper = month.toUpperCase();
                    // see if abbreviation matches any month
                    for (Month m : Month.values()) {
                        if (m.getDisplayName(TextStyle.SHORT, Locale.ENGLISH).equalsIgnoreCase(monthUpper)) {
                            monthNumber = m.getValue();
                            break;
                        }
                    }
                    if (monthNumber == -1) {
                        System.out.println("\nInvalid month abbreviation.");
                        return null;
                    }
                } else { // for full names or typos
                    try {
                        monthNumber = Month.valueOf(month.toUpperCase()).getValue();
                    } catch (IllegalArgumentException exception) {
                        System.out.println("\nInvalid month name. There may have been a typo.");
                        return null;
                    }
                }
            }
        }
        // Get first day of given month, format as dd-MM-yyyy and return as LocalDate
        LocalDate firstDayOfMonth = LocalDate.of(today.getYear(), monthNumber, 1);
        String formattedDate = firstDayOfMonth.format(DATE_FORMATTER);
        return LocalDate.parse(formattedDate, DATE_FORMATTER);
    }

    /**
     * Formats a String as String dd-MM-yyyy based on different input formats if possible.
     *
     * @param inputDate date to format as String
     * @return formatted date as String. Null + message if input doesn't match formats.
     */
    private static String formatDate(String inputDate) {
        String[] possibleFormats = { "dd.MM.yyyy", "dd/MM/yyyy", "dd-MM-yyyy", "ddMMyyyy" };
        LocalDate parsedDate = null;

        for (String format : possibleFormats) {
            try {
                parsedDate = LocalDate.parse(inputDate.trim(), DateTimeFormatter.ofPattern(format));
                break;
            } catch (Exception e) {
                // Continue trying the next format
            }
        }
        if (parsedDate != null) {
            return parsedDate.format(DateTimeFormatter.ofPattern("dd-MM-yyyy"));
        } else {
            System.out.println("\nInvalid date format. Please use one of the following formats: dd-MM-yyyy, ddMMyyyy, dd/MM/yyyy or dd.MM.yyyy.");
            return null;
        }
    }

    /**
     * Reverses (formats) a date order so e.g. dd-MM-yyyy becomes yyyy-MM-dd
     * @param date to be formatted
     * @return reverse order format.
     */
    private static String reverseDate(String date) {
        String[] partingDate = date.split("-", 3);
        return partingDate[2] + "-" + partingDate[1] + "-" + partingDate[0];
    }
}