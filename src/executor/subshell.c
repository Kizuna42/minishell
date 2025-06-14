/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:33:56 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_child_fds(t_minishell *shell)
{
	if (shell->stdin_backup >= 0)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	if (shell->stdout_backup >= 0)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
}

int	handle_subshell_signals(pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signal_handlers();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

int	execute_subshell(t_ast_node *node, t_minishell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_default_signal_handlers();
		cleanup_child_fds(shell);
		exit(execute_ast(node->left, shell));
	}
	else if (pid > 0)
		return (handle_subshell_signals(pid));
	return (1);
}
