# Chicken Bowl

The chicken bowl project is a adruino nano project. The idea is to provide fresh, filtered water to the chickens all year around with minimal effort. 

## Features

### Provide fresh water

This is achieved by pumping water into the bowl in a configurable interval such as every hour. 
We use a pump that consumes a voltage of 5V. 

- Water pump 6V

### Heat water during the winter

When the temperatur drops it is necessary o keep the water at a configurable temperature and heat it when it goes below the threshold. We use a heat pad that needs 12 V and therefor is controlled via relais. Additionally we need to frquently (every 5 min) read the temperature via digital pin. 

- Heat Pad 12 V
- Relais 5V -> 12V
- Temperature Probe + Controller
