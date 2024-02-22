#include "hal_data.h"
#include "stdio.h"
#include "lcd1602.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);
void console_write(const char *buffer);
void user_uart_callBack(uart_callback_args_t *p_args);
void hs300x_callback(rm_hs300x_callback_args_t *p_args);
static volatile bool isTransfer = false;
static volatile bool hs300x_complete = false;
static volatile bool g_ob1203_i2c_completed = false;

void ob1203_comms_i2c_callback(rm_ob1203_callback_args_t *p_args);
void ob1203_irq_callback(rm_ob1203_callback_args_t *p_args);
void lcd_i2c_callback(rm_comms_callback_args_t *p_args);
static volatile bool g_lcd_i2c_complete = false;
void delayCustom(uint32_t delay);
void delayCustomUS(uint32_t delay);
void delay500();

uint8_t x, y;
int16_t temp;
int16_t humid;
double percentageWater = 0;
double clear_data_percentage = 0;

const uint8_t smile[8] = {0x0E, 0x1F, 0x15, 0x1F, 0x15, 0x1B, 0x0E, 0x00}; // Array to generate the smiley character
uint8_t x, y;
int _write(int file, char *ptr, int len);

int _write(int file, char *ptr, int len)
{
  FSP_PARAMETER_NOT_USED(file); // We don't use this parameter

  for (int i = 0; i < len; i++) // Loop to send all the characters
  {
    if (*ptr == '\r') // If character is CR
    {
      x = 1;                // Then set the x coordinate to the first position
      lcd_set_cursor(x, y); // And update the cursor position
    }
    else if (*ptr == '\n') // If character is LF
    {
      if (y == 1)           // If we are in the first line
        y = 2;              // Then move to line 2
      lcd_set_cursor(x, y); // And update the cursor position
    }
    else                       // In all other cases
      lcd_data((uint8_t)*ptr); // Send the character to the LCD
    ptr++;                     // And increment the pointer
  }

  return len; // Return the number of characters that were sent
}

extern bsp_leds_t g_bsp_leds;

/*******************************************************************************************************************/ /**
                                                                                                                       * @brief  Blinky example application
                                                                                                                       *
                                                                                                                       * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
                                                                                                                       *
                                                                                                                       **********************************************************************************************************************/
void hal_entry(void)
{
#if BSP_TZ_SECURE_BUILD

  /* Enter non-secure code */
  R_BSP_NonSecureEnter();
#endif

  /* Define the units to be used with the software delay function */
  const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

  /* Set the blink frequency (must be <= bsp_delay_units */
  const uint32_t freq_in_hz = 3;

  /* Calculate the delay in terms of bsp_delay_units */
  const uint32_t delay = bsp_delay_units / freq_in_hz;

  /* LED type structure */
  bsp_leds_t leds = g_bsp_leds;

  /* If this board has no LEDs then trap here */
  if (0 == leds.led_count)
  {
    while (1)
    {
      ; // There are no LEDs on this board
    }
  }

  /* Holds level to set for pins */
  //    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

  fsp_err_t err = FSP_SUCCESS;
  rm_hs300x_raw_data_t hs300x_raw_data;
  rm_hs300x_data_t hs300x_data;
  uint8_t calculated_flag = 0;
  hs300x_complete = false;

  rm_ob1203_raw_data_t raw_data;
  rm_ob1203_light_data_t ob1203_light_data;
  rm_ob1203_prox_data_t ob1203_prox_data;
#if 0 == RM_OB1203_EXAMPLE_IRQ_ENABLE
                 rm_ob1203_device_status_t device_status;
#endif

  // KHOI TAO UART
  char write_buffer[100] = {};
  R_SCI_UART_Open(&g_write_UART_ctrl, &g_write_UART_cfg);

  /* Open the I2C bus if it is not already open. */
  rm_comms_i2c_bus_extended_cfg_t *p_extend_hs300x =
      (rm_comms_i2c_bus_extended_cfg_t *)g_hs300x_sensor0_cfg.p_instance->p_cfg->p_extend;
  i2c_master_instance_t *p_driver_instance = (i2c_master_instance_t *)p_extend_hs300x->p_driver_instance;
  p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
  RM_HS300X_Open(&g_hs300x_sensor0_ctrl, &g_hs300x_sensor0_cfg);

  /* Open the I2C bus if it is not already open. */
  rm_comms_i2c_bus_extended_cfg_t *p_extend_ob =
      (rm_comms_i2c_bus_extended_cfg_t *)g_ob1203_sensor0_cfg.p_comms_instance->p_cfg->p_extend;
  i2c_master_instance_t *p_driver_instance_ob = (i2c_master_instance_t *)p_extend_ob->p_driver_instance;
  p_driver_instance_ob->p_api->open(p_driver_instance_ob->p_ctrl, p_driver_instance_ob->p_cfg);
  RM_OB1203_Open(&g_ob1203_sensor0_ctrl, &g_ob1203_sensor0_cfg);

  // LCD I2C
  setvbuf(stdout, NULL, _IONBF, 2); // Disable Output Buffering
  x = 1;                            // Initial x coordinate
  y = 1;                            // Initial y coordinate

  lcd_init(0, 0); // Initialize the LCD without cursor and blinking
  lcd_set_cursor(x, y);
  printf("Hello World!!!");
  clearDisplay();

  const adc_channel_cfg_t g_adc0_channel_cfg =
      {
          .scan_mask = ADC_MASK_CHANNEL_0 | ADC_MASK_CHANNEL_1,
          .scan_mask_group_b = 0,
          .priority_group_a = (adc_group_a_t)0,
          .add_mask = 0,
          .sample_hold_mask = 0,
          .sample_hold_states = 0,
      };

  R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
  R_BSP_PinAccessEnable();
  uint32_t mode = D6;
  uint32_t count = 2;

  while (1)
  {

    sprintf(write_buffer, "\x1b[H");
    console_write(write_buffer);

    // ********************************************* TEMPERATURE SENSOR *********************************************//

    hs300x_complete = false;
    /* Start Measurement */
    RM_HS300X_MeasurementStart(&g_hs300x_sensor0_ctrl);
    while (hs300x_complete == false)
    {
      /* Wait callback */
    }
    do
    {
      hs300x_complete = false;
      /* Read ADC Data from HS300X */
      RM_HS300X_Read(&g_hs300x_sensor0_ctrl, &hs300x_raw_data);
      while (hs300x_complete == false)
      {
        /* Wait callback */
      }
      /* Calculate Humidity and Temperature values from ADC data */
      err = RM_HS300X_DataCalculate(&g_hs300x_sensor0_ctrl, &hs300x_raw_data, &hs300x_data);
      if (FSP_SUCCESS == err)
      {
        calculated_flag = 1;
      }
      else if (FSP_ERR_SENSOR_INVALID_DATA == err)
      {
        /* Stale data */
        calculated_flag = 0;
      }
      else
      {
      }
    } while (0 == calculated_flag);
    sprintf(write_buffer, "Humidity value: %d.%d \r \n", hs300x_data.humidity.integer_part, hs300x_data.humidity.decimal_part);
    console_write(write_buffer);
    if (count == 2)
    {

      x = 2;
      y = 1;
      lcd_set_cursor(x, y);

      printf("Humid: %d.%d%%  ", hs300x_data.humidity.integer_part, hs300x_data.humidity.decimal_part);
      //
      sprintf(write_buffer, "Temperature value: %d.%d \n \r", hs300x_data.temperature.integer_part, hs300x_data.temperature.decimal_part);
      console_write(write_buffer);
      x = 2;
      y = 2;
      lcd_set_cursor(x, y);
      //        temp = hs300x_data.temperature.integer_part;
      //    temp = temp - 6;
      //        humid = hs300x_data.humidity.decimal_part
      printf("Tempe: %d.%dC  ", hs300x_data.temperature.integer_part, hs300x_data.temperature.decimal_part);
    }
    // ******************************************************************************************

    // ********************************************* LIGHT SENSOR *********************************************//

    g_ob1203_i2c_completed = false;
    /* Start measurement in both Light and Proximity modes */
    RM_OB1203_MeasurementStart(&g_ob1203_sensor0_ctrl);
    while (g_ob1203_i2c_completed == false)
    {
      /* Wait callback */
    }
    /*
     * Example :
     * Device interrupt : Proximity mode
     * Measurement peroid(Light mode) : 50ms
     * Measurement peroid(Proximity mode) : 100ms
     */

    const uint32_t freq_in_hz_ob = 20;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay_ob = bsp_delay_units / freq_in_hz_ob;
    /* Delay 50ms for Light mode */
    R_BSP_SoftwareDelay(delay_ob, BSP_DELAY_UNITS_MILLISECONDS);
    g_ob1203_i2c_completed = false;
    /* Read Light ADC data */
    RM_OB1203_LightRead(&g_ob1203_sensor0_ctrl, &raw_data, RM_OB1203_LIGHT_DATA_TYPE_ALL);
    while (g_ob1203_i2c_completed == false)
    {
      /* Wait callback */
    }
    /* Calculate Light data */
    RM_OB1203_LightDataCalculate(&g_ob1203_sensor0_ctrl, &raw_data, &ob1203_light_data);

    sprintf(write_buffer, "Light: %lu \n \r", ob1203_light_data.clear_data);
    console_write(write_buffer);

#if RM_OB1203_EXAMPLE_IRQ_ENABLE
    /* Wait IRQ callback */
    while (0 == g_irq_flag)
    {
      /* Wait callback */
    }
    g_irq_flag = 0;
#else
    do
    {
      g_ob1203_i2c_completed = false;
      /* Get device status */
      RM_OB1203_DeviceStatusGet(&g_ob1203_sensor0_ctrl, &device_status);
      while (g_ob1203_i2c_completed == false)
      {
        /* Wait callback */
      }
    } while (false == device_status.prox_measurement_complete);
#endif
    g_ob1203_i2c_completed = false;
    /* Read Proximity ADC data */
    RM_OB1203_ProxRead(&g_ob1203_sensor0_ctrl, &raw_data);
    while (g_ob1203_i2c_completed == false)
    {
      /* Wait callback */
    }
    /* Calculate proximity data */
    RM_OB1203_ProxDataCalculate(&g_ob1203_sensor0_ctrl, &raw_data, &ob1203_prox_data);
    sprintf(write_buffer, "Proximity value: %d \n \r", ob1203_prox_data.proximity_data);
    console_write(write_buffer);
    // ******************************************************************************************

    R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    R_ADC_ScanStart(&g_adc0_ctrl);
    adc_status_t status;
    status.state = ADC_STATE_SCAN_IN_PROGRESS;
    while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
    {
      (void)R_ADC_StatusGet(&g_adc0_ctrl, &status);
    }
    uint16_t channel1_conversion_result;
    R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_1, &channel1_conversion_result);
    percentageWater = (double)(4095 - channel1_conversion_result) / 4095 * 100;
    sprintf(write_buffer, "ADC Value: %.2f%%\n", percentageWater);
    console_write(write_buffer);
    sprintf(write_buffer, "ADC Value: %d", channel1_conversion_result);
    console_write(write_buffer);

    if (count == 1)
    {
      x = 1;
      y = 1;
      lcd_set_cursor(x, y);
      clear_data_percentage = ((double)ob1203_light_data.clear_data / 15000) * 100;
      printf("  Light: %.2f%%                   ", clear_data_percentage);
      x = 1;
      y = 2;
      lcd_set_cursor(x, y);
      printf("  Soil : %.2f%%                 ", percentageWater);
    }

    bsp_io_level_t levelMode = R_BSP_PinRead(mode);
    if (levelMode == BSP_IO_LEVEL_LOW && count == 2)
    {
      count = 1;
      while (levelMode == BSP_IO_LEVEL_LOW)
      {
        levelMode = R_BSP_PinRead(mode);
      }
    }
    else if (levelMode == BSP_IO_LEVEL_LOW && count == 1)
    {
      count = 2;
      while (levelMode == BSP_IO_LEVEL_LOW)
      {
        levelMode = R_BSP_PinRead(mode);
      }
    }

    if ((hs300x_data.temperature.integer_part >= 30 && hs300x_data.humidity.integer_part <= 60) || percentageWater <= 40)
    {
      R_BSP_PinAccessEnable();
      uint32_t pinAudio = D3;
      uint32_t busyPin = D4;
      uint32_t pinWater = P006;
      uint32_t pinSoil = D5;

      R_BSP_PinCfg(pinSoil, BSP_IO_DIRECTION_INPUT);
      R_BSP_PinCfg(busyPin, BSP_IO_DIRECTION_INPUT);
      R_BSP_PinCfg(pinAudio, BSP_IO_DIRECTION_OUTPUT);
      R_BSP_PinCfg(pinWater, BSP_IO_DIRECTION_OUTPUT);

      bsp_io_level_t levelSoil = R_BSP_PinRead(pinSoil);
      bsp_io_level_t levelSensor;

      while (levelSoil == BSP_IO_LEVEL_HIGH)
      {
        levelSoil = R_BSP_PinRead(pinSoil);
        if (levelSoil == BSP_IO_LEVEL_LOW)
        {
          R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_LOW);
        }
        else if (levelSoil == BSP_IO_LEVEL_HIGH)
        {
          R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_HIGH);
          delayCustom(1111);
          R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_LOW);
          delayCustom(1000);
          R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_HIGH);
          delayCustom(1111);
          R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_LOW);
          delayCustom(1000);
          levelSoil = R_BSP_PinRead(pinSoil);
          if (levelSoil == BSP_IO_LEVEL_HIGH)
          {
            R_BSP_PinWrite(pinAudio, BSP_IO_LEVEL_HIGH);
            delay500();
            R_BSP_PinWrite(pinAudio, BSP_IO_LEVEL_LOW);
            delay500();
            levelSensor = R_BSP_PinRead(busyPin);
            while (levelSensor == BSP_IO_LEVEL_LOW)
            {
              levelSensor = R_BSP_PinRead(busyPin);
            }
          }
        }
      }
      R_BSP_PinWrite(pinWater, BSP_IO_LEVEL_LOW);
    }
    /* Delay */
    R_BSP_SoftwareDelay(delay, bsp_delay_units);
  }
}

/*******************************************************************************************************************/ /**
                                                                                                                       * This function is called at various points during the startup process.  This implementation uses the event that is
                                                                                                                       * called right before main() to set up the pins.
                                                                                                                       *
                                                                                                                       * @param[in]  event    Where at in the start up process the code is currently at
                                                                                                                       **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
  if (BSP_WARM_START_RESET == event)
  {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

    /* Enable reading from data flash. */
    R_FACI_LP->DFLCTL = 1U;

    /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
     * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
  }

  if (BSP_WARM_START_POST_C == event)
  {
    /* C runtime environment and system clocks are setup. */

    /* Configure pins. */
    IOPORT_CFG_OPEN(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
  }
}

void user_uart_callBack(uart_callback_args_t *p_args)
{
  switch (p_args->event)
  {
  case UART_EVENT_TX_COMPLETE:
  {
    isTransfer = true;
    break;
  }
  default:
  {
  }
  }
}

void hs300x_callback(rm_hs300x_callback_args_t *p_args)
{
  switch (p_args->event)
  {
  case RM_HS300X_EVENT_SUCCESS:
  {
    hs300x_complete = true;
    break;
  }
  default:
  {
  }
  }
}

void console_write(const char *buffer)
{
  isTransfer = false;
  R_SCI_UART_Write(&g_write_UART_ctrl, (uint8_t *)buffer, strlen(buffer));
  while (!isTransfer)
  {
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
  }
}

void ob1203_comms_i2c_callback(rm_ob1203_callback_args_t *p_args)
{
  switch (p_args->event)
  {
  case RM_OB1203_EVENT_SUCCESS:
  {
    g_ob1203_i2c_completed = true;
    break;
  }
  default:
  {
    /*Do nothing*/
  }
  }
}
void ob1203_irq_callback(rm_ob1203_callback_args_t *p_args)
{
#if ob1203_name_upper_IRQ_ENABLE
  FSP_PARAMETER_NOT_USED(p_args);
  g_ob1203_irq_completed = true;
#else
  FSP_PARAMETER_NOT_USED(p_args);
#endif
}

void delayCustom(uint32_t delay)
{
  /* Define the units to be used with the software delay function */
  const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

  //    /* Calculate the delay in terms of bsp_delay_units */
  //   const uint32_t delay = bsp_delay_units / freq_in_hz;

  /* Delay */
  R_BSP_SoftwareDelay(delay, bsp_delay_units);
}

void delayCustomUS(uint32_t delay)
{
  /* Define the units to be used with the software delay function */
  const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MICROSECONDS;

  /* Delay */
  R_BSP_SoftwareDelay(delay, bsp_delay_units);
}

void lcd_i2c_callback(rm_comms_callback_args_t *p_args)
{
  switch (p_args->event)
  {
  case (RM_COMMS_EVENT_OPERATION_COMPLETE):
  {
    g_lcd_i2c_complete = true;
    break;
  }
  default:
  {
  }
  }
}

void delay500()
{
  /* Define the units to be used with the software delay function */
  const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

  /* Set the blink frequency (must be <= bsp_delay_units */
  const uint32_t freq_in_hz = 2;

  /* Calculate the delay in terms of bsp_delay_units */
  const uint32_t delay = bsp_delay_units / freq_in_hz;
  R_BSP_SoftwareDelay(delay, bsp_delay_units);
}
