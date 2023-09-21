struct client_info *get_client(int s)
{
	struct client_info *ci = clients;
	while(ci) {
		if (ci->socket == s)
			break;
		ci = ci->next;
	}
	if (ci)
		return ci;
	struct client_info *n = (struct client_info*) calloc(1, sizeof(struct client_info));
	if (!n) {
		fprintf(stderr, "Out of memory.\n");
		exit(1);
	}
	n->address_length = sizeof(n->address);
	n->next = clients;
	clients = n;
	return n;
}
