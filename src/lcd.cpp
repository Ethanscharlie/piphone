#include "lcd.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define I2C_ADDR 0x27
#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04

int i2c_fd;

static void lcd_write_byte(uint8_t data, uint8_t mode);
static void lcd_write_cmd(uint8_t cmd);
static void lcd_write_data(uint8_t data);
static void lcd_init();
static void lcd_write_4bits(uint8_t data);
static void lcd_toggle_enable(uint8_t data);

static void lcd_write_byte(uint8_t data, uint8_t mode) {
  uint8_t high = mode | (data & 0xF0) | LCD_BACKLIGHT;
  uint8_t low = mode | ((data << 4) & 0xF0) | LCD_BACKLIGHT;
  lcd_write_4bits(high);
  lcd_write_4bits(low);
}

static void lcd_write_cmd(uint8_t cmd) { lcd_write_byte(cmd, 0x00); }

static void lcd_write_data(uint8_t data) {
  lcd_write_byte(data, 0x01); // RS = 1
}

static void lcd_write_4bits(uint8_t data) {
  write(i2c_fd, &data, 1);
  lcd_toggle_enable(data);
}

static void lcd_toggle_enable(uint8_t data) {
  usleep(500);
  uint8_t en = data | ENABLE;
  write(i2c_fd, &en, 1);
  usleep(500);
  en = data & ~ENABLE;
  write(i2c_fd, &en, 1);
  usleep(500);
}

static void lcd_init() {
  usleep(50000);
  lcd_write_4bits(0x30 | LCD_BACKLIGHT);
  usleep(4500);
  lcd_write_4bits(0x30 | LCD_BACKLIGHT);
  usleep(150);
  lcd_write_4bits(0x30 | LCD_BACKLIGHT);
  lcd_write_4bits(0x20 | LCD_BACKLIGHT);

  lcd_write_cmd(0x28); // Function set
  lcd_write_cmd(0x08); // Display off
  lcd_write_cmd(0x01); // Clear display
  usleep(2000);
  lcd_write_cmd(0x06); // Entry mode set
  lcd_write_cmd(0x0C); // Display on, cursor off
}

void LCD::init() {
  const char *device = "/dev/i2c-1";
  if ((i2c_fd = open(device, O_RDWR)) < 0) {
    throw std::logic_error("Failed to open I2C bus");
  }

  if (ioctl(i2c_fd, I2C_SLAVE, I2C_ADDR) < 0) {
    throw std::logic_error("Failed to connect to I2C device");
  }

  lcd_init();
}

void LCD::clearAndSet(std::string l1, std::string l2) {
  lcd_write_cmd(0x80); // Set cursor to first line
  const char *msg = l1.c_str();
  for (int i = 0; msg[i]; i++) {
    lcd_write_data(msg[i]);
  }
}
