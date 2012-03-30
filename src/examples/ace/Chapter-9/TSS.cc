#include<ace/config-lite.h>
#include<ace/Synch.h>
#include<ace/Task.h>
#include"ClientContext.hh"

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
void *ClientContext::get_attribute(const char *name) {
	void *value = 0;

	attributeMap_.find(name, value);
	return(value);
}


void ClientContext::set_attribute(const char *name, void *value) {
	attributeMap_.bind(name, value);
}


class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH> {
public:
	virtual int svc(void) {
		ACE_thread_t tid = this->thr_mgr()->thr_self();

		// Set our identifier in TSS.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Setting TSS TID: %u \n"), tid));
		this->tss_ctx_->set_attribute("thread_id", &tid);

		while (handle_requests() > 0) {
			;
		}

		return(0);
	}


	int handle_requests(void) {
		ACE_thread_t *tid = (ACE_thread_t *)this->tss_ctx_->get_attribute("thread_id");

		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Received TSS TID: %u \n"), *tid));

		// do work.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("Doing some work for thread %t\n")));
		return(-1);
	}


private:
	ACE_TSS<ClientContext> tss_ctx_;
};

int ACE_TMAIN(int, ACE_TCHAR *[]) {
	HA_CommandHandler handler;

	handler.activate(THR_NEW_LWP | THR_JOINABLE, 5);
	handler.wait();
	return(0);
}
