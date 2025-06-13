/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/13 22:48:45 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "../../includes/minishell.h"

static void	handle_sigint_child(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
}

static void	handle_sigquit_child(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "Quit (core dumped)\n", 19);
}

void	setup_child_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint_child;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit_child;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_default_signal_handlers(void)
{
	struct termios	term;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_cc[VQUIT] = '\x1C';
		term.c_lflag |= ISIG;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}
