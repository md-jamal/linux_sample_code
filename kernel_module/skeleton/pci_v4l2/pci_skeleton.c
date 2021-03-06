/*
 * This is a V4L2 PCI Skeleton Driver. It gives an initial skeleton source
 * for use with other PCI drivers.
 *
 * This skeleton PCI driver assumes that the card has an S-Video connector as
 * input 0 and an HDMI connector as input 1.
 *
 * Copyright 2014 Cisco Systems, Inc. and/or its affiliates. All rights reserved.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/mutex.h>
#include <linux/pci.h>
#include <linux/interrupt.h>




/**
 * struct skeleton - All internal data for one instance of device
 * @pdev: PCI device
 * @lock: ioctl serialization mutex
 */
struct skeleton {
	struct pci_dev *pdev;
	struct mutex lock;
};




static const struct pci_device_id skeleton_pci_tbl[] = {
	/* { PCI_DEVICE(PCI_VENDOR_ID_, PCI_DEVICE_ID_) }, */
	{ 0, }
};


/*
 * Interrupt handler: typically interrupts happen after a new frame has been
 * captured. It is the job of the handler to remove the new frame from the
 * internal list and give it back to the vb2 framework, updating the sequence
 * counter and timestamp at the same time.
 */
static irqreturn_t skeleton_irq(int irq, void *dev_id)
{
	printk("\n skeleton_irq");
	return IRQ_HANDLED;
}
/*
 * The initial setup of this device instance. Note that the initial state of
 * the driver should be complete. So the initial format, standard, timings
 * and video input should all be initialized to some reasonable value.
 */
extern int  vivi_init(void); 
static int skeleton_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	/* The initial timings are chosen to be 720p60. */
	struct skeleton *skel;
	int ret;

	/* Enable PCI */
	ret = pci_enable_device(pdev);
	if (ret)
		return ret;
	ret = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
	if (ret) {
		dev_err(&pdev->dev, "no suitable DMA available.\n");
		goto disable_pci;
	}

	/* Allocate a new instance */
	skel = devm_kzalloc(&pdev->dev, sizeof(struct skeleton), GFP_KERNEL);
	if (!skel)
		return -ENOMEM;

	/* Allocate the interrupt */
	ret = devm_request_irq(&pdev->dev, pdev->irq,
			       skeleton_irq, 0, KBUILD_MODNAME, skel);
	if (ret) {
		dev_err(&pdev->dev, "request_irq failed\n");
		goto disable_pci;
	}
	skel->pdev = pdev;

	ret = vivi_init();
	if (ret)
		goto disable_pci;

	mutex_init(&skel->lock);


	return 0;


disable_pci:
	pci_disable_device(pdev);
	return ret;
}

extern void vivi_exit(void);
static void skeleton_remove(struct pci_dev *pdev)
{
	vivi_exit();
	pci_disable_device(pdev);
}

static struct pci_driver skeleton_driver = {
	.name = KBUILD_MODNAME,
	.probe = skeleton_probe,
	.remove = skeleton_remove,
	.id_table = skeleton_pci_tbl,
};

MODULE_DESCRIPTION("PCI Skeleton Driver");
MODULE_AUTHOR("Channing Lan");
MODULE_LICENSE("GPL v2");
MODULE_DEVICE_TABLE(pci, skeleton_pci_tbl);


module_pci_driver(skeleton_driver);