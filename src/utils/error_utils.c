/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 18:05:22 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_directory_error(char *command)
{
	if (ft_strchr(command, '/'))
	{
		print_error(command, "is a directory");
		return (126);
	}
	else
	{
		return (127);
	}
}

int	handle_permission_error(char *command)
{
	if (ft_strchr(command, '/'))
	{
		print_error(command, "Permission denied");
		return (126);
	}
	else
	{
		return (127);
	}
}
