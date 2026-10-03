/*
* Copyright (c) 2016, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <i2c.h>
#include <stdio.h>
#include <lk/debug.h>

int i2c_transfer(i2cDev_t *i2c, struct i2c_msg *m, int n)
{
	return i2c->algo->master_xfer(i2c,m,n);
}

int i2c_send(int bus, uint32_t slaveAddr,
		uint8_t *buf, uint32_t num)
{
	int ret;
	struct i2c_msg msgs;

	i2cDev_t *i2c = (i2cDev_t *)&I2C_BUS[bus];
	msgs.buf = buf;
	msgs.len = num;
	msgs.flags = 0;
	msgs.addr = slaveAddr;
	ret = i2c_transfer(i2c, &msgs, 1);

	if (ret < 0)
		dprintf(INFO,"i2c transfer fail\n");

	return ret;
}

int i2c_receive(int bus, uint32_t slaveAddr,
		uint8_t *buf, uint32_t num)
{
	int ret;
	struct i2c_msg msgs;

	i2cDev_t *i2c = (i2cDev_t *)&I2C_BUS[bus];
	msgs.buf = buf;
	msgs.len = num;
	msgs.flags = I2C_M_RD;
	msgs.addr = slaveAddr;
	ret = i2c_transfer(i2c, &msgs, 1);

	if (ret < 0)
		dprintf(INFO,"i2c transfer fail\n");

	return ret;
}

int i2c_read_write(int bus, uint32_t slaveAddr,
		const uint8_t *wr_buff, uint32_t wr_size,
		uint8_t *rd_buff, uint32_t rd_size)
{
	int ret;
	struct i2c_msg msgs[] = {
		{
			.addr = slaveAddr,
			.flags = 0,
			.len = wr_size,
			.buf = (unsigned char *) wr_buff
		}, {
			.addr = slaveAddr,
			.flags = I2C_M_RD,
			.len = rd_size,
			.buf = rd_buff},
		};

	i2cDev_t *i2c = (i2cDev_t *)&I2C_BUS[bus];
	ret = i2c_transfer(i2c, msgs, 2);

	if (ret < 0)
		dprintf(INFO,"i2c transfer fail\n");

	return ret;
}

