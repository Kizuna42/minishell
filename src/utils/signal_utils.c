/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/11 17:19:52 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include "../../includes/minishell.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_status = SIGINT;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	handle_sigquit(int sig)
{
	(void)sig;
	return ;
}

static void	disable_quit_char(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_cc[VQUIT] = 0;
		term.c_lflag &= ~ISIG;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		term.c_lflag |= ISIG;
		term.c_cc[VQUIT] = _POSIX_VDISABLE;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sa.sa_handler = handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_sigquit;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa, NULL);
	disable_quit_char();
}
