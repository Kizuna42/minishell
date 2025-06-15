/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:06:13 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_left_child(t_ast_node *node, t_minishell *shell,
	int pipefd[2])
{
	setup_default_signal_handlers();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	cleanup_child_fds(shell);
	exit(execute_ast(node->left, shell));
}

static int	execute_right_child(t_ast_node *node, t_minishell *shell,
	int pipefd[2])
{
	setup_default_signal_handlers();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	cleanup_child_fds(shell);
	exit(execute_ast(node->right, shell));
}

static int	wait_and_handle_signals(pid_t left_pid, pid_t right_pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	setup_signal_handlers();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

int	execute_pipeline(t_ast_node *node, t_minishell *shell)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) == -1)
		return (1);
	left_pid = fork();
	if (left_pid == 0)
		execute_left_child(node, shell, pipefd);
	usleep(10000);
	right_pid = fork();
	if (right_pid == 0)
		execute_right_child(node, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_and_handle_signals(left_pid, right_pid));
}
