/*
 * s2mps20.c - mfd core driver for the s2mps20
 *
 * Copyright (C) 2016 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
//
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/mfd/core.h>
#include <linux/mfd/samsung/s2mps20.h>
#include <linux/mfd/samsung/s2mps20-regulator.h>
#include <linux/regulator/machine.h>
#include <linux/rtc.h>
#include <soc/samsung/acpm_mfd.h>

#ifdef CONFIG_OF
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#endif /* CONFIG_OF */

#define I2C_ADDR_TOP	0x00
#define I2C_ADDR_PMIC	0x01
#define I2C_ADDR_DEBUG	0x0F

static struct device_node *acpm_mfd_node;

static struct mfd_cell s2mps20_devs[] = {
	{ .name = "s2mps20-regulator", },
};

int s2mps20_read_reg(struct i2c_client *i2c, u8 reg, u8 *dest)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	u8 channel = 1;
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = exynos_acpm_read_reg(acpm_mfd_node, channel, i2c->addr, reg, dest);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret) {
		pr_err("[%s] acpm ipc fail!\n", __func__);
		return ret;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_read_reg);

int s2mps20_bulk_read(struct i2c_client *i2c, u8 reg, int count, u8 *buf)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	u8 channel = 1;
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = exynos_acpm_bulk_read(acpm_mfd_node, channel, i2c->addr, reg, count, buf);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret) {
		pr_err("[%s] acpm ipc fail!\n", __func__);
		return ret;
	}
	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_bulk_read);

int s2mps20_write_reg(struct i2c_client *i2c, u8 reg, u8 value)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	u8 channel = 1;
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = exynos_acpm_write_reg(acpm_mfd_node, channel, i2c->addr, reg, value);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret) {
		pr_err("[%s] acpm ipc fail!\n", __func__);
		return ret;
	}
	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_write_reg);

int s2mps20_bulk_write(struct i2c_client *i2c, u8 reg, int count, u8 *buf)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	u8 channel = 1;
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = exynos_acpm_bulk_write(acpm_mfd_node, channel, i2c->addr, reg, count, buf);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret) {
		pr_err("[%s] acpm ipc fail!\n", __func__);
		return ret;
	}
	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_bulk_write);

int s2mps20_update_reg(struct i2c_client *i2c, u8 reg, u8 val, u8 mask)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	u8 channel = 1;
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = exynos_acpm_update_reg(acpm_mfd_node, channel, i2c->addr, reg, val, mask);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret) {
		pr_err("[%s] acpm ipc fail!\n", __func__);
		return ret;
	}
	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_update_reg);

#if 0
int s2mps20_read_reg(struct i2c_client *i2c, u8 reg, u8 *dest)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_read_byte_data(i2c, reg);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0) {
		pr_info("%s:%s reg(0x%x), ret(%d)\n",
			 MFD_DEV_NAME, __func__, reg, ret);
		return ret;
	}

	ret &= 0xff;
	*dest = ret;
	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_read_reg);

int s2mps20_bulk_read(struct i2c_client *i2c, u8 reg, int count, u8 *buf)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_read_i2c_block_data(i2c, reg, count, buf);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0)
		return ret;

	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_bulk_read);

int s2mps20_read_word(struct i2c_client *i2c, u8 reg)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_read_word_data(i2c, reg);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0)
		return ret;

	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_read_word);

int s2mps20_write_reg(struct i2c_client *i2c, u8 reg, u8 value)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_write_byte_data(i2c, reg, value);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0)
		pr_info("%s:%s reg(0x%x), ret(%d)\n",
				MFD_DEV_NAME, __func__, reg, ret);

	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_write_reg);

int s2mps20_bulk_write(struct i2c_client *i2c, u8 reg, int count, u8 *buf)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_write_i2c_block_data(i2c, reg, count, buf);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0)
		return ret;

	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_bulk_write);

int s2mps20_write_word(struct i2c_client *i2c, u8 reg, u16 value)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_write_word_data(i2c, reg, value);
	mutex_unlock(&s2mps20->i2c_lock);
	if (ret < 0)
		return ret;
	return 0;
}
EXPORT_SYMBOL_GPL(s2mps20_write_word);

int s2mps20_update_reg(struct i2c_client *i2c, u8 reg, u8 val, u8 mask)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);
	int ret;
	u8 old_val, new_val;

	mutex_lock(&s2mps20->i2c_lock);
	ret = i2c_smbus_read_byte_data(i2c, reg);
	if (ret >= 0) {
		old_val = ret & 0xff;
		new_val = (val & mask) | (old_val & (~mask));
		ret = i2c_smbus_write_byte_data(i2c, reg, new_val);
	}
	mutex_unlock(&s2mps20->i2c_lock);
	return ret;
}
EXPORT_SYMBOL_GPL(s2mps20_update_reg);

#endif

#if defined(CONFIG_OF)
static int of_s2mps20_dt(struct device *dev,
			 struct s2mps20_platform_data *pdata,
			 struct s2mps20_dev *s2mps20)
{
	struct device_node *np = dev->of_node;
	int strlen;
	const char *status;
	//u32 val;

	if (!np)
		return -EINVAL;

	acpm_mfd_node = np;

	pdata->irq_gpio = of_get_named_gpio(np, "s2mps20,irq-gpio", 0);

	status = of_get_property(np, "s2mps20,wakeup", &strlen);
	if (status == NULL)
		return -EINVAL;
	if (strlen > 0) {
		if (!strcmp(status, "enabled") || !strcmp(status, "okay"))
			pdata->wakeup = true;
		else
			pdata->wakeup = false;
	}

	pr_info("%s: irq-gpio: %u \n", __func__, pdata->irq_gpio);

	if (of_get_property(np, "i2c-speedy-address", NULL))
		pdata->use_i2c_speedy = true;

	return 0;
}
#else
static int of_s2mps20_dt(struct device *dev,
			 struct s2mps20_platform_data *pdata)
{
	return 0;
}
#endif /* CONFIG_OF */

static int s2mps20_i2c_probe(struct i2c_client *i2c,
				const struct i2c_device_id *dev_id)
{
	struct s2mps20_dev *s2mps20;
	struct s2mps20_platform_data *pdata = i2c->dev.platform_data;

	u8 reg_data;
	int ret = 0;

	pr_info("%s:%s\n", MFD_DEV_NAME, __func__);

	s2mps20 = kzalloc(sizeof(struct s2mps20_dev), GFP_KERNEL);
	if (!s2mps20) {
		dev_err(&i2c->dev, "%s: Failed to alloc mem for s2mps20\n",
								 __func__);
		return -ENOMEM;
	}

	if (i2c->dev.of_node) {
		pdata = devm_kzalloc(&i2c->dev,
			 sizeof(struct s2mps20_platform_data), GFP_KERNEL);
		if (!pdata) {
			dev_err(&i2c->dev, "Failed to allocate memory\n");
			ret = -ENOMEM;
			goto err;
		}

		ret = of_s2mps20_dt(&i2c->dev, pdata, s2mps20);
		if (ret < 0) {
			dev_err(&i2c->dev, "Failed to get device of_node\n");
			goto err;
		}

		i2c->dev.platform_data = pdata;
	} else
		pdata = i2c->dev.platform_data;

	i2c->addr = 0x00; /* TODO : Force temp addr*/
	s2mps20->dev = &i2c->dev;
	s2mps20->i2c = i2c;
	s2mps20->irq = i2c->irq;
	s2mps20->device_type = S2MPS20X;

	if (pdata) {
		s2mps20->pdata = pdata;

		pdata->irq_base = irq_alloc_descs(-1, 0, S2MPS20_IRQ_NR, -1);
		if (pdata->irq_base < 0) {
			pr_err("%s:%s irq_alloc_descs Fail! ret(%d)\n",
				MFD_DEV_NAME, __func__, pdata->irq_base);
			ret = -EINVAL;
			goto err;
		} else
			s2mps20->irq_base = pdata->irq_base;

		s2mps20->irq_gpio = pdata->irq_gpio;
		s2mps20->wakeup = pdata->wakeup;
	} else {
		ret = -EINVAL;
		goto err;
	}
	mutex_init(&s2mps20->i2c_lock);

	i2c_set_clientdata(i2c, s2mps20);

	if (s2mps20_read_reg(i2c, S2MPS20_PMIC_REG_PMICID, &reg_data) < 0) {
		dev_err(s2mps20->dev,
			"device not found on this channel (this is not an error)\n");
		ret = -ENODEV;
		goto err_w_lock;
	} else {
		/* print rev */
		s2mps20->pmic_rev = reg_data & 0x07;
	}

	s2mps20->pmic = i2c_new_dummy(i2c->adapter, I2C_ADDR_PMIC);
	s2mps20->debug_i2c = i2c_new_dummy(i2c->adapter, I2C_ADDR_DEBUG);

	/* TODO: Not SPEEDY
	if (pdata->use_i2c_speedy) {
		dev_err(s2mps20->dev, "use_i2c_speedy was true\n");
		s2mps20->pmic->flags |= I2C_CLIENT_SPEEDY;
		s2mps20->debug_i2c->flags |= I2C_CLIENT_SPEEDY;
	}*/

	i2c_set_clientdata(s2mps20->pmic, s2mps20);
	i2c_set_clientdata(s2mps20->debug_i2c, s2mps20);

	pr_info("%s device found: rev.0x%2x\n", __func__, s2mps20->pmic_rev);

	ret = s2mps20_irq_init(s2mps20);
	if (ret < 0)
		goto err_irq_init;

	ret = mfd_add_devices(s2mps20->dev, -1, s2mps20_devs,
			ARRAY_SIZE(s2mps20_devs), NULL, 0, NULL);
	if (ret < 0)
		goto err_mfd;

	device_init_wakeup(s2mps20->dev, pdata->wakeup);

	return ret;

err_mfd:
	mfd_remove_devices(s2mps20->dev);
err_irq_init:
	i2c_unregister_device(s2mps20->i2c);
err_w_lock:
	mutex_destroy(&s2mps20->i2c_lock);
err:
	kfree(s2mps20);
	return ret;
}

static int s2mps20_i2c_remove(struct i2c_client *i2c)
{
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);

	mfd_remove_devices(s2mps20->dev);
	i2c_unregister_device(s2mps20->i2c);
	kfree(s2mps20);

	return 0;
}

static const struct i2c_device_id s2mps20_i2c_id[] = {
	{ MFD_DEV_NAME, TYPE_S2MPS20 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, s2mps20_i2c_id);

#if defined(CONFIG_OF)
static struct of_device_id s2mps20_i2c_dt_ids[] = {
	{ .compatible = "samsung,s2mps20mfd" },
	{ },
};
#endif /* CONFIG_OF */

#if defined(CONFIG_PM)
static int s2mps20_suspend(struct device *dev)
{
	struct i2c_client *i2c = container_of(dev, struct i2c_client, dev);
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);

	if (device_may_wakeup(dev))
		enable_irq_wake(s2mps20->irq);

	disable_irq(s2mps20->irq);

	return 0;
}

static int s2mps20_resume(struct device *dev)
{
	struct i2c_client *i2c = container_of(dev, struct i2c_client, dev);
	struct s2mps20_dev *s2mps20 = i2c_get_clientdata(i2c);

#if !defined(CONFIG_SAMSUNG_PRODUCT_SHIP)
	pr_info("%s:%s\n", MFD_DEV_NAME, __func__);
#endif /* CONFIG_SAMSUNG_PRODUCT_SHIP */

	if (device_may_wakeup(dev))
		disable_irq_wake(s2mps20->irq);

	enable_irq(s2mps20->irq);

	return 0;
}
#else
#define s2mps20_suspend	NULL
#define s2mps20_resume NULL
#endif /* CONFIG_PM */

const struct dev_pm_ops s2mps20_pm = {
	.suspend_late = s2mps20_suspend,
	.resume_early = s2mps20_resume,
};

static struct i2c_driver s2mps20_i2c_driver = {
	.driver		= {
		.name	= MFD_DEV_NAME,
		.owner	= THIS_MODULE,
#if defined(CONFIG_PM)
		.pm	= &s2mps20_pm,
#endif /* CONFIG_PM */
#if defined(CONFIG_OF)
		.of_match_table	= s2mps20_i2c_dt_ids,
#endif /* CONFIG_OF */
		.suppress_bind_attrs = true,
	},
	.probe		= s2mps20_i2c_probe,
	.remove		= s2mps20_i2c_remove,
	.id_table	= s2mps20_i2c_id,
};

static int __init s2mps20_i2c_init(void)
{
	pr_info("%s:%s\n", MFD_DEV_NAME, __func__);
	return i2c_add_driver(&s2mps20_i2c_driver);
}
/* init early so consumer devices can complete system boot */
subsys_initcall(s2mps20_i2c_init);

static void __exit s2mps20_i2c_exit(void)
{
	i2c_del_driver(&s2mps20_i2c_driver);
}
module_exit(s2mps20_i2c_exit);

MODULE_DESCRIPTION("s2mps20 multi-function core driver");
MODULE_AUTHOR("Samsung Electronics");
MODULE_LICENSE("GPL");
