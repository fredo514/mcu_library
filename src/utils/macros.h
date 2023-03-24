#ifndef MACROS_H
#define	MACROS_H

#define SET_BIT(REG, BIT)     ((REG) |= (1<<(BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1<<(BIT)))
#define READ_BIT(REG, BIT)    ((REG) & (1<<(BIT)))

#define SET_MASK(REG, MASK)     ((REG) |= (MASK))
#define CLEAR_MASK(REG, BIT)   ((REG) &= ~(MASK))
#define READ_MASK(REG, MASK)    ((REG) & (MASK))

#endif // MACROS_H