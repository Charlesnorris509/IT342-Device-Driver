//Charles NOrris Device DRiver
//IT342 Spring 2024


#include <linux/kernel.h>           // Include the kernel header file for kernel-specific functions and definitions
#include <linux/module.h>           // Include the kernel module header file for module-related functions and definitions
#include <linux/miscdevice.h>       // Include the miscellaneous device header file for miscellaneous device support
#include <linux/fs.h>               // Include the file system header file for file system-related functions and definitions

static int example_open(struct inode *inode, struct file *file)
{
    pr_info("Open Device\n");   // Print a message indicating that the device is being opened
    return 0;                           // Return 0 to indicate successful opening of the device
}

static int example_close(struct inode *inodep, struct file *filp)
{
    pr_info("Closing device\n"); // Print a message indicating that the device is being closed
    return 0;                            // Return 0 to indicate successful closing of the device
}

static ssize_t write(struct file *file, const char __user *buf,
                       size_t len, loff_t *ppos)
{
    // Write function to handle data transfer from user space to kernel space
    // In this example, it simply prints the length of the data being written
    ssize_t ret;

    // Ensure the data length does not exceed the buffer size
    if (len > BUFFER_SIZE) {
        pr_err("Data length exceeds buffer size\n");
        return -EINVAL; // Invalid argument error
    }

    // Copy data from userspace to kernel space
    ret = copy_from_user(kernel_buffer, buf, len);
    if (ret != 0) {
        pr_err("Failed to copy data from userspace\n");
        return ret;
    }

    pr_info("Rec
}

static ssize_t read(struct file *file, char __user *buf,
                       size_t len, loff_t *ppos)
{
    // Read function to handle data transfer from kernel space to user space
    // In this example, it simply prints the length of the data being read
	ssize_t ret;

    // Ensure there is data available to read
    if (*ppos >= BUFFER_SIZE) {
        pr_info("No more data to read\n");
        return 0;
    }

    // Calculate the amount of data to read
    size_t data_len = min(len, (size_t)(BUFFER_SIZE - *ppos));

    // Copy data from kernel space to userspace
    ret = copy_to_user(buf, kernel_buffer + *ppos, data_len);
    if (ret != 0) {
        pr_err("Failed to copy data to userspace\n");
        return ret;
    }

    // Update the file position
    *ppos += data_len;

    pr_info("Sent data to userspace: %s\n", kernel_buffer + *ppos);
    return data_len;
}

static const struct file_operations example_fops = {
    .owner                      = THIS_MODULE,       // Owner of the file operations structure
    .open                       = example_open,     // Open function callback
    .release                    = example_close,    // Close function callback
    .write                      = example_write,    // Write function callback
    .read                       = example_read,     // Read function callback
};

struct miscdevice example_device = {
    .minor = MISC_DYNAMIC_MINOR,   // Dynamically allocate a minor number
    .name = "misc_device",         // Name of the miscellaneous device
    .fops = &example_fops,         // File operations for the device
};

static int __init misc_init(void)
{
    int error;

    error = misc_register(&example_device);  // Register the miscellaneous device
    if (error) {
        pr_err("Unable to register Driver\n"); // Print an error message if registration fails
        return error;                                   // Return the error code
    }

    pr_info("Registered Driver\n"); // Print a message indicating successful registration
    return 0;                               // Return 0 to indicate successful initialization
}

static void __exit misc_exit(void)
{
    misc_deregister(&example_device);       // Deregister the miscellaneous device
    pr_info("Deregistered Driver\n"); // Print a message indicating successful deregistration
}

module_init(misc_init) // Specify the initialization function for the module
module_exit(misc_exit) // Specify the exit function for the module

MODULE_DESCRIPTION("IT342 Device Driver Report -> {CODE}"); // Module description
MODULE_AUTHOR("Charles Norris <cnorris7@gmu.edu>");         // Module author information
